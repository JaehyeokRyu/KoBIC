map<int, int> GetChMap(void)
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

void jbnu_daq_ana(const int RunNo=50191, const char* inPath = "./BICDAQ"){

	gStyle->SetOptStat(0);

	const int nch = 32;
	const int tmin = 30; 
	const int tmax = 50; 

	const int cutPed = 80;

	std::map<int, int> chMap = GetChMap();

	const char* inFile = Form("%s/Run_%d_Anc_MID_31_FILE_0.root", inPath, RunNo);
	TFile *infile = new TFile(inFile, "read");

	TTree *T = (TTree*)infile->Get("T");

	int v_adc[nch][500] = {0};
	T->SetBranchAddress("adc", v_adc);

	TH1D *h1DPulse[nch];
	for (int ich=0; ich<nch; ich++){
		h1DPulse[ich] = new TH1D(Form("h1DPulse_%d",ich),"",500,0,500);
	}

	TH1D *h1Dx = new TH1D("h1Dx","h1Dx",nch/2,0,nch/2);
	TH1D *h1Dy = new TH1D("h1Dy","h1Dy",nch/2,0,nch/2);

	TH1D *h1DxMax = new TH1D("h1DxMax","",1500,0,1500);
	TH1D *h1DyMax = new TH1D("h1DyMax","",1500,0,1500);

	TH2D *h2DnHit = new TH2D("h2DnHit","",11,-0.5,10.5,11,-0.5,10.5);

	TH2D *h2DHitMap = new TH2D("h2DHitMap","",nch/2,-0.5,nch/2-0.5,nch/2,-0.5,nch/2-0.5);

	int nEvt = T->GetEntries();

	for (int iEvt=0; iEvt<nEvt; iEvt++){

		for (int ich=0; ich<nch; ich++){
			h1DPulse[ich]->Reset();
		}//ich

		T->GetEntry(iEvt);

		int nhitx = 0;
		int nhity = 0;

		for (int ich=0; ich<nch; ich++){
			for (int ibin=tmin; ibin<tmax; ibin++){

				h1DPulse[ich]->SetBinContent(ibin+1, v_adc[ich][ibin]);

			}//ibin

			if ( ich<nch/2 ){
				h1Dx->SetBinContent(ich+1, h1DPulse[ich]->GetMaximum()); 

				if ( h1DPulse[ich]->GetMaximum()>cutPed ){
					nhitx++;
				}
			}else{
				h1Dy->SetBinContent(ich-(nch/2)+1, h1DPulse[ich]->GetMaximum()); 

				if ( h1DPulse[ich]->GetMaximum()>cutPed ){
					nhity++;
				}
			}
		}//ich

		h2DnHit->Fill(nhitx, nhity);

		h1DxMax->Fill(h1Dx->GetMaximum());
		h1DyMax->Fill(h1Dy->GetMaximum());

		if ( nhitx==1 && nhity==1 ){
			int chx = chMap[h1Dx->GetMaximumBin()] - 1;
			int chy = chMap[h1Dy->GetMaximumBin()] - 1;
			h2DHitMap->Fill(chx, chy);
		}

	}//iEvt

	TCanvas *c1 = new TCanvas("c1", "c1", 1.2*3*300, 2*300);
	c1->Divide(3,2);
	c1->cd(1);
	gPad->SetTicks();
	gPad->SetMargin(0.14,0.03,0.12,0.02);
	h1DxMax->SetLineColor(1);
	h1DxMax->SetLineWidth(2);
	h1DxMax->GetXaxis()->SetRangeUser(0, 800);
	h1DxMax->GetXaxis()->SetTitleSize(0.05);;
	h1DxMax->GetXaxis()->SetLabelSize(0.045);;
	h1DxMax->GetXaxis()->SetTitle("ADC peak");
	h1DxMax->GetYaxis()->SetTitleSize(0.05);;
	h1DxMax->GetYaxis()->SetLabelSize(0.045);;
	h1DxMax->GetYaxis()->SetTitle("Number of events");
	h1DxMax->Draw();

	h1DyMax->SetLineColor(2);
	h1DyMax->SetLineWidth(2);
	h1DyMax->Draw("same");

	c1->cd(2);
	gPad->SetMargin(0.14,0.12,0.12,0.02);
	gPad->SetTicks();
	h2DnHit->GetXaxis()->SetTitleSize(0.05);;
	h2DnHit->GetXaxis()->SetLabelSize(0.045);;
	h2DnHit->GetXaxis()->SetTitle("Number of hits in X");
	h2DnHit->GetYaxis()->SetTitleSize(0.05);;
	h2DnHit->GetYaxis()->SetLabelSize(0.045);;
	h2DnHit->GetYaxis()->SetTitle("Number of hits in Y");
	h2DnHit->Draw("colz");

	c1->cd(4);
	gPad->SetMargin(0.14,0.12,0.12,0.02);
	gPad->SetTicks();
	h2DHitMap->GetXaxis()->SetTitleSize(0.05);;
	h2DHitMap->GetXaxis()->SetLabelSize(0.045);;
	h2DHitMap->GetXaxis()->SetTitle("Hit position in X");
	h2DHitMap->GetYaxis()->SetTitleSize(0.05);;
	h2DHitMap->GetYaxis()->SetLabelSize(0.045);;
	h2DHitMap->GetYaxis()->SetTitle("Hit position in Y");
	h2DHitMap->Draw("colz");

	c1->cd(5);
	gPad->SetMargin(0.14,0.03,0.12,0.02);
	gPad->SetTicks();
	TH1D *h2DHitMapx = (TH1D*)h2DHitMap->ProjectionX("h2DHitMapx");
	h2DHitMapx->GetXaxis()->SetTitleSize(0.05);;
	h2DHitMapx->GetXaxis()->SetLabelSize(0.045);;
	h2DHitMapx->GetXaxis()->SetTitle("Hit position in X");
	h2DHitMapx->GetYaxis()->SetTitleSize(0.05);;
	h2DHitMapx->GetYaxis()->SetLabelSize(0.045);;
	h2DHitMapx->Draw("");

	int nEvtHit = int(h2DnHit->Integral()-h2DnHit->GetBinContent(h2DnHit->FindBin(0,0)));

	{
		TF1 *fx = new TF1("fx","gaus",6,16);
		h2DHitMapx->Fit(fx,"R0Q");
		fx->Draw("same");
		TLegend *leg = new TLegend(0.20,0.95-0.06*2,0.5,0.95); 
		leg->SetBorderSize(0);
		leg->AddEntry("",Form("RUN %d, Hodoscope X",RunNo),"h");
		leg->AddEntry("",Form("mu=%4.1f, #sigma=%4.1f", fx->GetParameter(1), fx->GetParameter(2)),"h");
		leg->SetTextSize(0.05);
		leg->Draw();
	}

	c1->cd(6);
	gPad->SetMargin(0.14,0.03,0.12,0.02);
	gPad->SetTicks();
	TH1D *h2DHitMapy = (TH1D*)h2DHitMap->ProjectionY("h2DHitMapy");
	h2DHitMapy->GetXaxis()->SetTitleSize(0.05);;
	h2DHitMapy->GetXaxis()->SetLabelSize(0.045);;
	h2DHitMapy->GetXaxis()->SetTitle("Hit position in Y");
	h2DHitMapy->GetYaxis()->SetTitleSize(0.05);;
	h2DHitMapy->GetYaxis()->SetLabelSize(0.045);;
	h2DHitMapy->Draw("");

	{
		TF1 *fy = new TF1("fy","gaus",7,16);
		h2DHitMapy->Fit(fy,"R0Q");
		fy->Draw("same");
		TLegend *leg = new TLegend(0.20,0.95-0.06*2,0.5,0.95); 
		leg->SetBorderSize(0);
		leg->AddEntry("",Form("RUN %d, Hodoscope Y",RunNo),"h");
		leg->AddEntry("",Form("mu=%4.1f, #sigma=%4.1f", fy->GetParameter(1), fy->GetParameter(2)),"h");
		leg->SetTextSize(0.05);
		leg->Draw();
	}

	c1->cd(3);
	TLegend *leg = new TLegend(0.10,0.3,0.5,0.7); 
	leg->SetBorderSize(0);
	leg->AddEntry("",Form("RUN %d, Hodoscope",RunNo),"h");
	leg->AddEntry("",Form("# of events: %d",nEvt),"h"); 
	leg->AddEntry("",Form("# of events with hit: %d",nEvtHit),"h"); 
	leg->AddEntry("",Form("Rate: %4.2f",1.0*nEvtHit/nEvt),"h"); 
	leg->SetTextSize(0.08);
	leg->Draw();

}
