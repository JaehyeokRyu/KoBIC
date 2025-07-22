#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TString.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstring>
using namespace std;

int GetDataLength(const char* inFile)
{
    ifstream in(inFile, ios::binary);
    if (!in.is_open()) {
        cout << "GetDataLength - cannot open the file! Stop.\n";
        return 1;
    }
    char hdr[32];
    in.read(hdr, 32);
    unsigned long len = 0;
    for (int i = 0; i < 4; ++i)
        len |= (unsigned long)(hdr[4*i] & 0xFF) << (8*i);
    in.close();
    return (int)len;
}

// mode: "o" (overlay), "a" (avg Time), "i" (int ADC), "p" (peak ADC), or "all"
void NKFADC_QA(int RunNo = 60265,
               int nEvtToRead = 10000,
               const char* inPath = "/Users/jay.ryu/KoBIC/25KEKDATA",
               int mid = 1,
               const char* mode = "all")
{
    const int nChan = 4;
    TString path = TString::Format(
      "%s/Run_%d/Run_%d_MID_%d/FADCData_%d_%d.dat",
      inPath, RunNo, RunNo, mid, mid, RunNo);
    const char* file = path.Data();
    cout << "Start NKFADC QA: " << file << "\n";

    FILE* fin = fopen(file, "rb");
    if (!fin) {
        cerr << "Cannot open " << file << "\n";
        return;
    }

    int DLen = GetDataLength(file);
    if (DLen <= 32) {
        cerr << "Invalid data length: " << DLen << "\n";
        fclose(fin);
        return;
    }
    // if (DLen > 16384) DLen = ((mid==3||mid==4)?2048:256);
    if (DLen > 16384) DLen = 2048;
    int TLen = DLen * nChan;
    int nADC = (DLen - 32) / 2;

    vector<unsigned char> raw(TLen);
    vector<vector<unsigned char>> dataChop(nChan, vector<unsigned char>(DLen));
    vector<int> samples(nADC);

    vector<TH1F*>     hPeak(nChan), hInt(nChan);
    vector<TProfile*> pAvg(nChan);
    vector<TH2F*>     hOvl(nChan);
    for (int ch = 0; ch < nChan; ++ch) {
        if (mid == 1 && ch == 0) continue;
        hPeak[ch] = new TH1F(Form("hPeak_m%d_ch%d", mid, ch+1),
                             Form("MID%d CH%d Peak", mid, ch+1),
                             200, 0, 2000);
        hInt[ch]  = new TH1F(Form("hInt_m%d_ch%d", mid, ch+1),
                             Form("MID%d CH%d Integral", mid, ch+1),
                             200, -1e5, 1e6);
        pAvg[ch]  = new TProfile(Form("hA_m%d_ch%d", mid, ch+1),
                                 Form("MID%d CH%d AvgTime", mid, ch+1),
                                 nADC, 0, nADC, -200, 12000);
        hOvl[ch]  = new TH2F(Form("hO_m%d_ch%d", mid, ch+1),
                             Form("MID%d CH%d Overlay", mid, ch+1),
                             nADC, 0, nADC,
                             4096, -0.5, 4095.5);
        hOvl[ch]->SetDirectory(0);
        pAvg[ch]->SetDirectory(0);
    }

    size_t data_read = 0;
    int processed = 0;

    while (processed < nEvtToRead) {

        size_t got = fread(raw.data(), 1, TLen, fin);
        if (got != (size_t)TLen) break;
        data_read += got;


        for (int k = 0; k < DLen; ++k)
            for (int ch = 0; ch < nChan; ++ch)dataChop[ch][k] = raw[ch + nChan * k];

        // bool valid = true;
        // unsigned long prevTrig = 0;
        // for (int ch = 0; ch < nChan; ++ch) {
        //     unsigned long dlen = 0;
        //     for (int b = 0; b < 4; ++b)
        //         dlen |= (unsigned long)(dataChop[ch][b] & 0xFF) << (8*b);
        //     if (dlen != (unsigned long)DLen) { valid = false; break; }

        //     unsigned int ttype = dataChop[ch][6] & 0x0F;
        //     if (ttype != 3) { valid = false; break; }

        //     unsigned long tnum = 0;
        //     for (int b = 0; b < 4; ++b)
        //         tnum |= (unsigned long)(dataChop[ch][7+b] & 0xFF) << (8*b);
        //     if (ch == 0) prevTrig = tnum;
        //     else if (tnum != prevTrig) { valid = false; break; }
        // }
        // if (!valid) continue;

        for (int ch = 0; ch < nChan; ++ch) {
            if (mid == 1 && ch == 0) continue;
            double ped = 0;
            int pedN = min(nADC, 40);
            for (int a = 0; a < nADC; ++a) {
                int idx = 32 + 2 * a;
                int lo = dataChop[ch][idx] & 0xFF;
                int hi = (dataChop[ch][idx + 1] & 0x0F) << 8;
                samples[a] = lo | hi;
                if (a < pedN) ped += samples[a];
            }
            ped /= pedN;

            if (!strcmp(mode, "o") || !strcmp(mode, "a") || !strcmp(mode, "all")) {
                for (int a = 0; a < nADC; ++a) {
                    hOvl[ch]->Fill(a, samples[a]);
                    pAvg[ch]->Fill(a, samples[a]);
                }
            }
            

            double peak = 0, integ = 0;
            for (int a = 0; a < nADC; ++a) {
                double corr = samples[a] - ped;
                peak = max(peak, corr);
                integ += corr;
            }

            if (!strcmp(mode, "p") || !strcmp(mode, "all"))
                hPeak[ch]->Fill(peak);
            if (!strcmp(mode, "i") || !strcmp(mode, "all"))
                hInt[ch]->Fill(integ);
        }

        ++processed;
    }

    fclose(fin);
    cout << "Total bytes read: " << data_read
         << ", Events processed: " << processed << "\n";


    if (!strcmp(mode, "o") || !strcmp(mode, "all")) {
        TCanvas* cO = new TCanvas("cOverlay", "Overlay", 900,700);
        cO->Divide(2,2);
        int pad = 1;
        for (int ch = 0; ch < nChan; ++ch) {
            if (mid==1 && ch==0) continue;
            cO->cd(pad++);
            hOvl[ch]->Draw("COLZ");
        }
    }
    
    
    if (!strcmp(mode, "a") || !strcmp(mode, "all")) {
        TCanvas* cA = new TCanvas("cAvgTime", "AvgTime", 900,700);
        cA->Divide(2,2);
        int pad = 1;
        for (int ch = 0; ch < nChan; ++ch) {
            if (mid==1 && ch==0) continue;
            cA->cd(pad++);
            pAvg[ch]->Draw();
        }
    }

    if (!strcmp(mode, "p") || !strcmp(mode, "all")) {
        TCanvas* cP = new TCanvas("cPeak", "Peak ADC", 900,700);
        cP->Divide(2,2);
        int pad=1;
        for (int ch = 0; ch < nChan; ++ch) {
            if (mid==1 && ch==0) continue;
            cP->cd(pad++);
            hPeak[ch]->Draw();
        }
    }

    if (!strcmp(mode, "i") || !strcmp(mode, "all")) {
        TCanvas* cI = new TCanvas("cInt", "Integral ADC", 900,700);
        cI->Divide(2,2);
        int pad=1;
        for (int ch = 0; ch < nChan; ++ch) {
            if (mid==1 && ch==0) continue;
            cI->cd(pad++);
            hInt[ch]->Draw();
        }
    }
}
