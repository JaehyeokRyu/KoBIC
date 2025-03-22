#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"

#include <bitset>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

#define bDEBUG false 



//for drawing
map<int, int> wform_factor = {{31, 100}, {41, 1500}, {42, 1500}};
map<int, int> cutPed = {{1, 50}, {2, 50}, {31, 20}, {41, 20}, {42, 20}};

const float xCVS = 1.0; //Adjust canvas size
const int nPacketCheck = 1000000;

TCanvas *ctmp;
TCanvas *c1;
TCanvas *c2;
TCanvas *c3;
TCanvas *c4[2];
TCanvas *c5[2];
TCanvas *c6[2];

void bic_daq_quickQA(int RunNo, int nEvtToRead, const char* inPath, int mid);

TH2D *H2_pulse[2][32];
TH1D *H1_peak[2][32];
TH1D *H1_sum[2][32];

map<int, int> GetHodoChMap(void)
{
    std::map<int, int> chMap;

    chMap.insert( std::pair<int, int> ( 1,  1) );
    chMap.insert( std::pair<int, int> ( 2,  2) );
    chMap.insert( std::pair<int, int> ( 3,  3) );
    chMap.insert( std::pair<int, int> ( 4,  4) );
    chMap.insert( std::pair<int, int> ( 5,  8) );
    chMap.insert( std::pair<int, int> ( 6,  7) );
    chMap.insert( std::pair<int, int> ( 7,  6) );
    chMap.insert( std::pair<int, int> ( 8,  5) );
    chMap.insert( std::pair<int, int> ( 9, 12) );
    chMap.insert( std::pair<int, int> (10, 11) );
    chMap.insert( std::pair<int, int> (11, 10) );
    chMap.insert( std::pair<int, int> (12,  9) );
    chMap.insert( std::pair<int, int> (13, 13) );
    chMap.insert( std::pair<int, int> (14, 14) );
    chMap.insert( std::pair<int, int> (15, 15) );
    chMap.insert( std::pair<int, int> (16, 16) );

    chMap.insert( std::pair<int, int> ( 1+16,  1+16) );
    chMap.insert( std::pair<int, int> ( 2+16,  2+16) );
    chMap.insert( std::pair<int, int> ( 3+16,  3+16) );
    chMap.insert( std::pair<int, int> ( 4+16,  4+16) );
    chMap.insert( std::pair<int, int> ( 5+16,  8+16) );
    chMap.insert( std::pair<int, int> ( 6+16,  7+16) );
    chMap.insert( std::pair<int, int> ( 7+16,  6+16) );
    chMap.insert( std::pair<int, int> ( 8+16,  5+16) );
    chMap.insert( std::pair<int, int> ( 9+16, 12+16) );
    chMap.insert( std::pair<int, int> (10+16, 11+16) );
    chMap.insert( std::pair<int, int> (11+16, 10+16) );
    chMap.insert( std::pair<int, int> (12+16,  9+16) );
    chMap.insert( std::pair<int, int> (13+16, 13+16) );
    chMap.insert( std::pair<int, int> (14+16, 14+16) );
    chMap.insert( std::pair<int, int> (15+16, 15+16) );
    chMap.insert( std::pair<int, int> (16+16, 16+16) );

    return chMap;
}//map

map<int, int> GetCaloChMap(void)
{

	std::map<int, int> chMap;

	chMap.insert( std::pair<int, int> (4101, 1001) );
	chMap.insert( std::pair<int, int> (4102, 2001) );
	chMap.insert( std::pair<int, int> (4103, 1006) );
	chMap.insert( std::pair<int, int> (4104, 2006) );
	chMap.insert( std::pair<int, int> (4105, 2005) );
	chMap.insert( std::pair<int, int> (4106, 2026) );
	chMap.insert( std::pair<int, int> (4107, 2030) );
	chMap.insert( std::pair<int, int> (4108, 2029) );
	chMap.insert( std::pair<int, int> (4109, 2003) );
	chMap.insert( std::pair<int, int> (4110, 2018) );
	chMap.insert( std::pair<int, int> (4111, 2017) );
	chMap.insert( std::pair<int, int> (4112, 2031) );
	chMap.insert( std::pair<int, int> (4113, 2002) );
	chMap.insert( std::pair<int, int> (4114, 2027) );
	chMap.insert( std::pair<int, int> (4115, 2022) );
	chMap.insert( std::pair<int, int> (4116, 2028) );
	chMap.insert( std::pair<int, int> (4117, 1007) );
	chMap.insert( std::pair<int, int> (4118, 2007) );
	chMap.insert( std::pair<int, int> (4119, 1015) );
	chMap.insert( std::pair<int, int> (4120, 2015) );
	chMap.insert( std::pair<int, int> (4121, 1005) );
	chMap.insert( std::pair<int, int> (4122, 1026) );
	chMap.insert( std::pair<int, int> (4123, 1030) );
	chMap.insert( std::pair<int, int> (4124, 1029) );
	chMap.insert( std::pair<int, int> (4125, 1003) );
	chMap.insert( std::pair<int, int> (4126, 1018) );
	chMap.insert( std::pair<int, int> (4127, 1017) );
	chMap.insert( std::pair<int, int> (4128, 1031) );
	chMap.insert( std::pair<int, int> (4129, 1002) );
	chMap.insert( std::pair<int, int> (4130, 1027) );
	chMap.insert( std::pair<int, int> (4131, 1022) );
	chMap.insert( std::pair<int, int> (4132, 1028) );

	chMap.insert( std::pair<int, int> (4201, 2008) );
	chMap.insert( std::pair<int, int> (4202, 2009) );
	chMap.insert( std::pair<int, int> (4203, 2024) );
	chMap.insert( std::pair<int, int> (4204, 2012) );
	chMap.insert( std::pair<int, int> (4205, 2014) );
	chMap.insert( std::pair<int, int> (4206, 2013) );
	chMap.insert( std::pair<int, int> (4207, 2021) );
	chMap.insert( std::pair<int, int> (4208, 2010) );
	chMap.insert( std::pair<int, int> (4209, 2004) );
	chMap.insert( std::pair<int, int> (4210, 2025) );
	chMap.insert( std::pair<int, int> (4211, 2023) );
	chMap.insert( std::pair<int, int> (4212, 2032) );
	chMap.insert( std::pair<int, int> (4213, 2011) );
	chMap.insert( std::pair<int, int> (4214, 2033) );
	chMap.insert( std::pair<int, int> (4215, 2020) );
	chMap.insert( std::pair<int, int> (4216, 2019) );
	chMap.insert( std::pair<int, int> (4217, 1008) );
	chMap.insert( std::pair<int, int> (4218, 1009) );
	chMap.insert( std::pair<int, int> (4219, 1024) );
	chMap.insert( std::pair<int, int> (4220, 1012) );
	chMap.insert( std::pair<int, int> (4221, 1014) );
	chMap.insert( std::pair<int, int> (4222, 1013) );
	chMap.insert( std::pair<int, int> (4223, 1021) );
	chMap.insert( std::pair<int, int> (4224, 1010) );
	chMap.insert( std::pair<int, int> (4225, 1004) );
	chMap.insert( std::pair<int, int> (4226, 1025) );
	chMap.insert( std::pair<int, int> (4227, 1023) );
	chMap.insert( std::pair<int, int> (4228, 1032) );
	chMap.insert( std::pair<int, int> (4229, 1011) );
	chMap.insert( std::pair<int, int> (4230, 1033) );
	chMap.insert( std::pair<int, int> (4231, 1020) );
	chMap.insert( std::pair<int, int> (4232, 1019) );

	return chMap;
}

int GetDataLength(const char* inFile)
{
	//Reading 1st event, 1st header (32 bytes fixed), 1st channel's very first four bits will be enough
	ifstream in;
	in.open(inFile, std::ios::binary);
	if (!in.is_open()) { cout <<"GetDataLength - cannot open the file! Stop.\n"; return 1; }

	char data[32];
	in.read(data, 32);
	unsigned long dataLength = 0;
	for (int i=0; i<4; i++) dataLength += ((ULong_t)(data[4*i] & 0xFF) << 8*i);

	in.close();
	return (int)dataLength;
}//GetDataLength

void quickQA_for_HV(int RunNo = 2080, int nEvtToRead = 10000, const char* inPath = "./25KEKDATA"){

	//gStyle->SetOptStat(0);
	gStyle->SetTitleSize(0.04);
	gStyle->SetTextSize(0.1); 
	gStyle->SetStatW(0.4); gStyle->SetStatH(0.3);

	vector<int> mid = {41, 42};

	if ( bDEBUG ){
		ctmp = new TCanvas("ctmp", "ctmp", 600, 500);
	}

	for (int ii=0; ii<2; ii++){
		for (int ich=0; ich<32; ich++){
			H2_pulse[ii][ich] = new TH2D(Form("H2_pulse_mid%d_ch%d",41+ii,1+ich),"",120,0,120,6000,-100,12000);

			H1_peak[ii][ich] = new TH1D(Form("H1_peak_mid%d_ch%d", 41+ii,1+ich),"",10000,-100,20000);
			H1_sum[ii][ich] = new TH1D(Form("H1_sum_mid%d_ch%d", 41+ii,1+ich),"",10000,-100,10000*15);
		}//ich
	}//ii

	c4[0] = new TCanvas("c4_0", Form("RUN %d, LEFT", RunNo), -1, 0, 200*8, 200*4);
	c4[0]->SetMargin(0,0,0,0);
	c4[0]->Divide(8,4,0,0);
	c4[1] = new TCanvas("c4_1", Form("RUN %d, RIGHT", RunNo), -1, 0, 200*8, 200*4);
	c4[1]->SetMargin(0,0,0,0);
	c4[1]->Divide(8,4,0,0);

	c5[0] = new TCanvas("c5_0", Form("RUN %d, PEAK, LEFT", RunNo), -1, 0, 200*8, 200*4);
	c5[0]->SetMargin(0,0,0,0);
	c5[0]->Divide(8,4,0,0);
	c5[1] = new TCanvas("c5_1", Form("RUN %d, PEAK, RIGHT", RunNo), -1, 0, 200*8, 200*4);
	c5[1]->SetMargin(0,0,0,0);
	c5[1]->Divide(8,4,0,0);

	c6[0] = new TCanvas("c6_0", Form("RUN %d, INTADC, LEFT", RunNo), -1, 0, 200*8, 200*4);
	c6[0]->SetMargin(0,0,0,0);
	c6[0]->Divide(8,4,0,0);
	c6[1] = new TCanvas("c6_1", Form("RUN %d, INTADC, RIGHT", RunNo), -1, 0, 200*8, 200*4);
	c6[1]->SetMargin(0,0,0,0);
	c6[1]->Divide(8,4,0,0);

	c1 = new TCanvas(Form("c1"), Form("RUN %d, Channel QA", RunNo), -1, 0, 80*1.2*16*xCVS, 80*7*xCVS); 
	gPad->SetMargin(0,0,0,0);
	c1->Divide(16,7,0,0);

	c2 = new TCanvas("c2", Form("RUN %d, DAQ Trigger QA", RunNo), -1, 0, 200*1.2*7*xCVS, 200*2*xCVS); 
	gPad->SetMargin(0,0,0,0);
	c2->Divide(7,2,0,0);

	c3 = new TCanvas("c3", Form("RUN %d, Packet QA", RunNo), -1, 0, 200*2*2*xCVS, 200*5*xCVS);
	gPad->SetMargin(0,0,0,0);
	c3->Divide(2,5,0,0);

	if ( find(mid.begin(), mid.end(), 42)!=mid.end() ){
		bic_daq_quickQA(RunNo, nEvtToRead, inPath, 42);
	}
	if ( find(mid.begin(), mid.end(), 41)!=mid.end() ){
		bic_daq_quickQA(RunNo, nEvtToRead, inPath, 41);
	}

	TH2D *H2CaloCoord = new TH2D("", "", 8, 0, 8, 4, 0, 4);
	H2CaloCoord->Fill(0., 0., 8);
	H2CaloCoord->Fill(1., 0., 14);
	H2CaloCoord->Fill(2., 0., 4);
	H2CaloCoord->Fill(3., 0., 11);
	H2CaloCoord->Fill(4., 0., 5);
	H2CaloCoord->Fill(5., 0., 3);
	H2CaloCoord->Fill(6., 0., 2);
	H2CaloCoord->Fill(7., 0., 1);

	H2CaloCoord->Fill(0., 1., 9);
	H2CaloCoord->Fill(1., 1., 13);
	H2CaloCoord->Fill(2., 1., 25);
	H2CaloCoord->Fill(3., 1., 33);
	H2CaloCoord->Fill(4., 1., 26);
	H2CaloCoord->Fill(5., 1., 18);
	H2CaloCoord->Fill(6., 1., 27);
	H2CaloCoord->Fill(7., 1., 6);

	H2CaloCoord->Fill(0., 2., 24);
	H2CaloCoord->Fill(1., 2., 21);
	H2CaloCoord->Fill(2., 2., 23);
	H2CaloCoord->Fill(3., 2., 20);
	H2CaloCoord->Fill(4., 2., 30);
	H2CaloCoord->Fill(5., 2., 17);
	H2CaloCoord->Fill(6., 2., 22);
	H2CaloCoord->Fill(7., 2., 7);

	H2CaloCoord->Fill(0., 3., 12);
	H2CaloCoord->Fill(1., 3., 10);
	H2CaloCoord->Fill(2., 3., 32);
	H2CaloCoord->Fill(3., 3., 19);
	H2CaloCoord->Fill(4., 3., 29);
	H2CaloCoord->Fill(5., 3., 31);
	H2CaloCoord->Fill(6., 3., 28);
	H2CaloCoord->Fill(7., 3., 15);

	for (int bb=0; bb<4; bb++){
		for (int aa=0; aa<8; aa++){
			cout << aa << " " << bb << " " << int(H2CaloCoord->GetBinContent(H2CaloCoord->FindBin(aa+0.5,bb+0.5))) << endl;
		}
	}

	std::map<int, int> chMapCalo = GetCaloChMap();

	for (int ii=0; ii<2; ii++){
		for (int ich=0; ich<32; ich++){

			int val = chMapCalo[(41+ii)*100 + ich + 1];
			int modid = val%100;

			int lrid = (val/100==10) ? 0 : 1;

			if ( lrid==0 ){
				cout << "mid: " << 41+ii << ", ch: " << ich+1 << ", L" << modid << endl;
			}else if ( lrid==1 ){
				cout << "mid: " << 41+ii << ", ch: " << ich+1 << ", R" << modid << endl;
			}

			for (int aa=0; aa<8; aa++){
				for (int bb=0; bb<4; bb++){

					if ( int(H2CaloCoord->GetBinContent(H2CaloCoord->FindBin(aa+0.5,bb+0.5)))==modid ){

						c4[lrid]->cd( aa + 1 + 8*(3 - bb));
						H2_pulse[ii][ich]->Draw("colz same");
						//cout << ii << " " << ich << " " << H2_pulse[ii][ich]->Integral() << endl;

						c5[lrid]->cd( aa + 1 + 8*(3 - bb));
						H1_peak[ii][ich]->Draw("same");

						c6[lrid]->cd( aa + 1 + 8*(3 - bb));
						H1_sum[ii][ich]->Draw("same");
						break;
					}

				}//bb
			}//aa


		}//ich
	}//ii

	return;
}

int GetBoardNumber(const char* inFile){
	ifstream in;
	in.open(inFile, std::ios::binary);
	if (!in.is_open()) { cout <<"GetDataLength - cannot open the file! Stop.\n"; return 1; }

	char data;
	in.read(&data, 1);
	int bid = data & 0xFF;

	return bid;
}




/*
map<str*, vector<int>> GetHodoChMap(void)
{
	std::map<int, int> chMap;

	// L측 (모듈 ID 41)
    {“L1”,  {41,  1}}, {“R1",  {41,  2}}, {“L6”,  {41,  3}}, {“R6",  {41,  4}},
    {“R5”,  {41,  5}}, {“R26",  {41,  6}}, {“R30”,  {41,  7}}, {“R29",  {41,  8}},
    {“R3”,  {41,  9}}, {“R18", {41, 10}}, {“R17”, {41, 11}}, {“R31", {41, 12}},
    {“R2”, {41, 13}}, {“R27", {41, 14}}, {“R22”, {41, 15}},
    {“R28", {41, 16}}, {“L7”, {41, 17}}, {“R7", {41, 18}}, {“L15”, {41, 19}},
    {“R15", {41, 20}}, {“L5”, {41, 21}}, {“L26", {41, 22}}, {“L30”, {41, 23}},
    {“L29", {41, 24}}, {“L3”, {41, 25}}, {“L18", {41, 26}}, {“L17”, {41, 27}},
    {“L31", {41, 28}}, {“L2”, {41, 29}}, {“L27", {41, 30}}, {“L22”, {41, 31}},
    {“L28", {41, 32}},
    // R측 (모듈 ID 42)
    {“R8”,  {42,  1}}, {“R9",  {42,  2}}, {“R24”,  {42,  3}}, {“12",  {42,  4}},
    {“R14”,  {42,  5}}, {“R13",  {42,  6}}, {“R21”,  {42,  7}}, {“R10",  {42,  8}},
    {“R4”,  {42,  9}}, {“R25", {42, 10}}, {“R23”, {42, 11}}, {“R32", {42, 12}},
    {“R111”, {42, 13}}, {“R33", {42, 14}}, {“R20”, {42, 15}},
    {“R19", {42, 16}}, {“L8”, {42, 17}}, {“L9", {42, 18}}, {“L24”, {42, 19}},
    {“L12", {42, 20}}, {“L14”, {42, 21}}, {“L13", {42, 22}}, {“L21”, {42, 23}},
    {“L10", {42, 24}}, {“L4”, {42, 25}}, {“L25", {42, 26}}, {“L23”, {42, 27}},
    {“L32", {42, 28}}, {“L11”, {42, 29}}, {“L33", {42, 30}}, {“L20”, {42, 31}},
    {“L19", {42, 32}}
};
*/

//--------------------------------------------------------------------------------------------------------
void bic_daq_quickQA(int RunNo = 2080, int nEvtToRead = 10000, const char* inPath = "data_hodoscope_raw", int mid = 42)
{


	//gStyle->SetPalette(kRainBow);

	const char* inFile;
	if ( mid==31 ){
		//inFile = Form("%s/jbnu_daq_%d_%i.dat", inPath, mid, RunNo);
		inFile = Form("%s/Run_%d/Run_%d_MID_%d/jbnu_daq_%d_%i.dat", inPath, RunNo, RunNo, mid, mid, RunNo);
	}else{
		//inFile = Form("%s/bic_daq_%d_%i.dat", inPath, mid, RunNo);
		inFile = Form("%s/Run_%d/Run_%d_MID_%d/bic_daq_%d_%i.dat", inPath, RunNo, RunNo, mid, mid, RunNo);
	}
	cout <<Form("Start quick QA by directly decoding %s...\n", inFile);

	//return;

	// Cuts, Containers
	//----------------------------------------------------------
	const int nCh = 32;
	std::map<int, int> chMapHodo = GetHodoChMap();
	std::map<int, int> chMapCalo = GetCaloChMap();

	int nHit[nCh] = {0};

	TH1F* H1_pulse = new TH1F(Form("H1_pulse_%d",mid), "", 2500, 0, 2500);
	for (int a=0; a<nCh; a++)
	{
		if ( mid==41 ){
			c1->cd(16*1+a+1);
		}else if ( mid==42 ){
			c1->cd(16*3+a+1);
		}else if ( mid==31 ){
			c1->cd(16*5+a+1);
		}
		gPad->SetTicks();
		gPad->SetMargin(0.14,0.02,0.1,0.01);
		TH1F* H1Temp = new TH1F(Form("RUN %d, MID %d, CH%d", RunNo, mid, a+1), "", 2500, 0, 2500);
		if ( mid==31 ){
			H1Temp->GetYaxis()->SetRangeUser(-1*wform_factor[mid], 10*wform_factor[mid]);
			H1Temp->GetXaxis()->SetRangeUser(0, 124);
		}else{
			H1Temp->GetYaxis()->SetRangeUser(-1*wform_factor[mid], 10*wform_factor[mid]);
			H1Temp->GetXaxis()->SetRangeUser(0, 124);
		}
		//H1Temp->GetXaxis()->SetTitle("Time bin");
		H1Temp->GetXaxis()->SetTitleSize(0.05*1.4);
		H1Temp->GetXaxis()->SetLabelSize(0.045*1.4);
		//H1Temp->GetYaxis()->SetTitle("ADC");
		H1Temp->GetYaxis()->SetTitleSize(0.05*1.4);
		H1Temp->GetYaxis()->SetLabelSize(0.045*1.4);
		H1Temp->GetYaxis()->SetTitleOffset(1.2);
		H1Temp->DrawCopy();

		c4[0]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.1,0.1,0.01);
		gPad->SetLogz();
		H1Temp->DrawCopy();

		c4[1]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.1,0.1,0.01);
		gPad->SetLogz();
		H1Temp->DrawCopy();

		c5[0]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.05,0.1,0.01);

		c5[1]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.05,0.1,0.01);

		c6[0]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.05,0.1,0.01);

		c6[1]->cd(a+1);
		gPad->SetTicks();
		gPad->SetMargin(0.15,0.05,0.1,0.01);
	}

	int trigN = -1;
	unsigned long long trigT = -1;
	TH2F* H2 = new TH2F(Form("trig_nVSt_%d",mid), Form("; Trigger Number; Trigger Time"), 10000,0,10000, 200,0,0.5*1.E10);
	TH2F* H2Local = new TH2F(Form("localtrig_nVSt_%d",mid), Form("; Trigger Number; Trigger Time"), 10000,0,10000, 200,0,0.5*1.E10);

	TH1F *H1Packet = new TH1F(Form("PacketQA_%d",mid), "", nPacketCheck, 0, nPacketCheck);

	#if 1 
	// Processing
	//----------------------------------------------------------

	//Get data file size (the size should be < 2 GB)
	FILE *fp = fopen(inFile, "rb");
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	fclose(fp);

	//Variables
	char header[32];
	char data[10000];
	int data_read = 0;

	//Open data file and read event by event
	fp = fopen(inFile, "rb");
	int nPacketProcessed=0;
	while (data_read < file_size)
	{
		//Read header
		//++++++++++++++++++++++++++++++++++++++++++++

		fread(header, 1, 32, fp);

		int data_length = 0;
		for (int a=0; a<4; a++) data_length += ((int)(header[a] & 0xFF) << 8*a);

		/*
		int run_number = 0;
		for (int a=0; a<2; a++) run_number += ((int)(header[a+4] & 0xFF) << 8*a);
		int trigger_type = ((int)header[6] & 0xFF);
		*/

		int tcb_trigger_number = 0;
		for (int a=0; a<4; a++) tcb_trigger_number += ((int)(header[a+7] & 0xFF) << 8*a);

		int tcb_trigger_fine_time = ((int)header[11] & 0xFF);
		int tcb_trigger_coarse_time = 0;
		for (int a=0; a<3; a++) tcb_trigger_coarse_time += ((int)(header[a+12] & 0xFF) << 8*a);
		unsigned long long tcb_trigger_time = (tcb_trigger_fine_time * 8) + (tcb_trigger_coarse_time * 1000);

		//int mid = ((int)header[15] & 0xFF);
		int channel = ((int)header[16] & 0xFF);

		/*
		int local_trigger_number = 0;
		for (int a=0; a<4; a++) local_trigger_number += ((int)(header[a+17] & 0xFF) << 8*a);

		int local_gate_fine_time = ((int)header[25] & 0xFF);

		unsigned long long local_gate_coarse_time = 0;
		for (int a=0; a<6; a++) local_gate_coarse_time += ((int)(header[a+26] & 0xFF) << 8*a);

		unsigned long long local_gate_time = (local_gate_fine_time * 8) + (local_gate_coarse_time * 1000);
		*/

		if ( channel>nCh ){
			cout << "WARNNING! suspicious channel number! MID: " << mid << ", CH:" << channel << endl; 
			fread(data, 1, 128*4 - 32, fp);
			nPacketProcessed++;
			continue;
		}

		/*
		if ( mid==41 && ((channel>=1 && channel<=4) || (channel>=16 && channel<=20)) ){
			fread(data, 1, 512 - 32, fp);
			nPacketProcessed++;
			continue;
		}
		*/

		if ( bDEBUG ){
			cout << "INFO: " << mid 
				<< " " << tcb_trigger_number 
				<< " " << tcb_trigger_time 
				//<< " " << local_trigger_number 
				//<< " " << local_gate_coarse_time 
				<< ", channel "  << channel 
				<< ", data length " << data_length 
				<< endl;
		}

		if ( fabs(tcb_trigger_number-trigN)>10000 ){
			cout << "WARNNING! suspicious tcb trigger number! MID: " << mid <<", TCB TrigN: " << tcb_trigger_number << " " << trigN 
				<< ", CH: " << channel << endl;
			fread(data, 1, 128*4 - 32, fp);
			nPacketProcessed++;
			continue;
		}

		if ( nPacketProcessed<H1Packet->GetNbinsX() ){
			H1Packet->SetBinContent(nPacketProcessed+1, 100*tcb_trigger_number + channel); 
		}

		if ( !(data_length==256 || data_length==128*4) ){
			if ( channel==0 ){
				fread(data, 1, 256 - 32, fp);
			}else{
				fread(data, 1, 128*4 - 32, fp);
			}
			cout << "WARNNING! suspicious data length! MID: " << mid << ", DLength: " << data_length << ", CH: " << channel << endl;
			nPacketProcessed++;
			continue;
		}

		if ( trigN!=tcb_trigger_number ){
			if ( trigT!=tcb_trigger_time ){
				trigN = tcb_trigger_number;
				trigT = tcb_trigger_time;
			}else{
				cout << "WARNNING! different trigger number but same trigger time!" << endl;
			}
		}

		//H2 Fill
		H2->Fill(tcb_trigger_number, tcb_trigger_time);
		//H2Local->Fill(local_trigger_number, local_gate_time);

		int wave_length = (mid==31) ? (data_length - 32) / 2 : (data_length - 32) / 4;

		//Read body, data_length - 32 bytes (header)
		//++++++++++++++++++++++++++++++++++++++++++++

		fread(data, 1, data_length - 32, fp);

#if 1
		int charge[32] = {0};
		int timing[32] = {0};
		int hitFlag[32] = {0};
		int adc[wave_length];
		for (int a=0; a<wave_length; a++) adc[a] = 0;

		if (channel == 0) //Spectrum data
		{
			/*
			for (int ch=0; ch<32; ch++)
			{
				for (int a=0; a<3; a++) charge[ch] += ((int)(data[6 * ch + a + 0] & 0xFF) << 8*a);
				for (int a=0; a<2; a++) timing[ch] += ((int)(data[6 * ch + a + 3] & 0xFF) << 8*a);
				hitFlag[ch] = data[6 * ch + 5] & 0xFF;
			}//a
			*/
		}
		else //Waveform data
		{
			H1_pulse->Reset();
			bool validPulse = false;
			if ( mid==31 ){
				for (int a=0; a<wave_length; a++)
				{
					int t_adc1 = (data[2 * a + 0] & 0xFF);
					int t_adc2 = (data[2 * a + 1] & 0xFF) << 8;
					adc[a] = (short)(t_adc1 + t_adc2);

					if (fabs(adc[a]) > cutPed[mid]) validPulse = true;
					H1_pulse->SetBinContent(a+1, adc[a]);
				}//a
			}else{
				for (int a=0; a<wave_length; a++)
				{
					int t_adc1 = (data[4 * a + 0] & 0xFF);
					int t_adc2 = (data[4 * a + 1] & 0xFF) << 8;
					adc[a] = (short)(t_adc1 + t_adc2);

					if (fabs(adc[a]) > cutPed[mid]) validPulse = true;
					H1_pulse->SetBinContent(a+1, adc[a]);
					H2_pulse[mid-41][channel-1]->Fill(a, adc[a]);
				}
			}

			H1_sum[mid-41][channel-1]->Fill(H1_pulse->Integral(51,100));
			H1_pulse->GetXaxis()->SetRange(51,100);
			H1_peak[mid-41][channel-1]->Fill(H1_pulse->GetMaximum());

			if (validPulse)
			{
				const int ch = (mid==31) ? chMapHodo[channel] : channel;
				nHit[ch-1]++;
				if ( mid==41 ){
					c1->cd(16*1+ch);
				}else if ( mid==42 ){
					c1->cd(16*3+ch);
				}else if ( mid==31 ){
					c1->cd(16*5+ch);
				}
				H1_pulse->DrawCopy("same");
			}
		}
#endif

		data_read = data_read + data_length;

		//++++++++++++++++++++++++++++++++++++++++++++

		nPacketProcessed++;
		if (nPacketProcessed%10000 == 0) cout << "Processed eventNum = " << nPacketProcessed/(nCh+1) << endl;
		if ((nPacketProcessed/(nCh+1)) == nEvtToRead) break;

	}
	#endif

	fclose(fp);

	//Draw
	TH1D *H1_rate = new TH1D("", "", nCh, 0, nCh);
	for (int a=0; a<nCh; a++){
		if ( mid==41 ){
			c1->cd(16*1+a+1);
		}else if ( mid==42 ){
			c1->cd(16*3+a+1);
		}else if ( mid==31 ){
			c1->cd(16*5+a+1);
		}

		TLegend *leg = new TLegend(0.2, 0.95-0.1*2, 0.5, 0.95);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextFont(43);
		leg->SetTextSize(8);
		leg->AddEntry("",Form("MID %d, CH %d", mid, a+1),"h"); 
		leg->AddEntry("",Form("Hit rate %d / %d = %1.2f",nHit[a],trigN,nHit[a]*1.0/trigN),"h");

		H1_rate->SetBinContent(a+1, nHit[a]*1.0/trigN);
		leg->Draw();
	}

	TH1D *H2_projx = (TH1D*)H2->ProjectionX("H2_projx");
	if ( bDEBUG ){
		ctmp->cd();
		H2_projx->Draw();
	}

	if ( mid==41 ){
		c2->cd(3);
	}else if ( mid==42 ){
		c2->cd(4);
	}else{
		c2->cd(5);
	}
	gPad->SetMargin(0.15,0.12,0.12,0.05);
	gPad->SetTicks();
	H2->GetXaxis()->SetRangeUser(0, trigN);
	H2->GetXaxis()->SetTitleSize(0.05);
	H2->GetXaxis()->SetLabelSize(0.045);
	H2->GetYaxis()->SetTitleSize(0.05);
	H2->GetYaxis()->SetLabelSize(0.045);
	H2->GetZaxis()->SetTitleSize(0.05);
	H2->GetZaxis()->SetLabelSize(0.045);
	H2->GetZaxis()->SetRangeUser(0, 1.5*nCh);
	H2->DrawCopy("colz");

	/*
	H2Local->GetXaxis()->SetRangeUser(0, trigN);
	H2Local->GetXaxis()->SetTitleSize(0.05);
	H2Local->GetXaxis()->SetLabelSize(0.045);
	H2Local->GetYaxis()->SetTitleSize(0.05);
	H2Local->GetYaxis()->SetLabelSize(0.045);
	H2Local->GetZaxis()->SetTitleSize(0.05);
	H2Local->GetZaxis()->SetLabelSize(0.045);
	H2Local->GetZaxis()->SetRangeUser(0, 1.5*nCh);
	H2Local->DrawCopy("colz");
	*/

	int trigN_GOOD = 0;
	for (int ii=0; ii<trigN*0.9; ii++){
		if ( H2_projx->GetBinContent(ii+1)==(nCh+1) ){
			trigN_GOOD++;
		}
	}

	TLegend *leg = new TLegend(0.15, 0.85-0.06*3, 0.5, 0.85);
	leg->SetFillStyle(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(43);
	leg->SetTextSize(12);
	leg->AddEntry("",Form("RUN %d, MID%d", RunNo, mid),"h");
	leg->AddEntry("",Form("Good events %d / %d = %4.1f%%",trigN_GOOD,int(trigN*0.9+1),trigN_GOOD*100/(trigN*0.9+1)),"h");
	if ( trigN_GOOD/(trigN*0.9+1)<0.9 ){
		leg->AddEntry("","Bad RUN!","h");
	}else{
		leg->AddEntry("","Good RUN!","h");
	}
	leg->Draw();

	if ( mid==41 ){
		c2->cd(7+3);
	}else if ( mid==42 ){
		c2->cd(7+4);
	}else{
		c2->cd(7+5);
	}
	gPad->SetTicks();
	gPad->SetMargin(0.14,0.01,0.12,0.12);
	H1_rate->GetYaxis()->SetRangeUser(0, 1.5*H1_rate->GetMaximum());
	H1_rate->GetXaxis()->SetTitle("CH Index");
	H1_rate->GetXaxis()->SetTitleSize(0.05);
	H1_rate->GetXaxis()->SetLabelSize(0.045);
	H1_rate->GetYaxis()->SetTitle("HIT Rate");
	H1_rate->GetYaxis()->SetTitleSize(0.05);
	H1_rate->GetYaxis()->SetLabelSize(0.045);
	H1_rate->GetZaxis()->SetTitleSize(0.05);
	H1_rate->GetZaxis()->SetLabelSize(0.045);
	H1_rate->DrawCopy("HIST");

	if ( 1 ){
		if ( mid==41 ){
			c3->cd(3);
		}else if ( mid==42 ){
			c3->cd(4);
		}else{
			c3->cd(5);
		}

		gPad->SetMargin(0.10,0.03,0.12,0.05);
		gPad->SetTicks();
		H1Packet->Draw();
		H1Packet->GetXaxis()->SetTitle("Packet Index");
		H1Packet->GetXaxis()->SetTitleSize(0.055);
		H1Packet->GetXaxis()->SetLabelSize(0.05);
		H1Packet->GetXaxis()->SetRangeUser(0, nPacketProcessed);
		H1Packet->GetYaxis()->SetTitle("Trigger Index*100 + Channel Index");
		H1Packet->GetYaxis()->SetTitleSize(0.055);
		H1Packet->GetYaxis()->SetTitleOffset(0.9);
		H1Packet->GetYaxis()->SetLabelSize(0.05);

		TLegend *leg = new TLegend(0.2, 0.95-0.08*1, 0.5, 0.95);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextFont(43);
		leg->SetTextSize(16);
		leg->AddEntry("",Form("RUN %d, MID %d", RunNo, mid),"h"); 
		leg->Draw();
	}


}

