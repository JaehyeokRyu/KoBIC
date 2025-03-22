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

void Fit(){

	const int nset = 9;

	int run[nset] = {124, 125, 126, 127, 128, 129, 130, 131, 132};
	int hv[nset]  = {500, 550, 600, 650, 700, 750, 800, 850, 900};

	TFile *infile[nset];
	TGraph *gHV[2][32];

	for (int ii=0; ii<2; ii++){
		for (int ich=0; ich<32; ich++){
			gHV[ii][ich] = new TGraph;
		}
	}

	for (int iset=0; iset<nset; iset++){

		infile[iset] = new TFile(Form("outfile_Run60%d.root",run[iset]),"read");

		for (int ii=0; ii<2; ii++){
			for (int ich=0; ich<32; ich++){

				TH1D *H1 = (TH1D*)infile[iset]->Get(Form("H1_sum_mid4%d_ch%d",ii+1,ich+1));

				//gHV[ii][ich]->SetPoint(iset, hv[iset], log(H1->GetMean()));
				gHV[ii][ich]->SetPoint(iset, hv[iset], H1->GetMean());
				gHV[ii][ich]->SetMarkerStyle(24);
				gHV[ii][ich]->SetMarkerSize(1);

			}
		}

	}//iset

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

	TCanvas *c4[2];

	for (int ii=0; ii<2; ii++){
		c4[ii] = new TCanvas(Form("c4_%d",ii), Form("LR %d",ii), -1, 0, 200*8, 200*4);
		c4[ii]->SetMargin(0,0,0,0);
		c4[ii]->Divide(8,4,0,0);
	}

	float intADC_val[8] = {7362.5, 32557.8, 48000, 41976, 27920, 16015, 8510, 0};

	for (int ii=0; ii<2; ii++){

		for (int ich=0; ich<32; ich++){

			if ( ii==0 && (ich<4) ) continue;

			int val = chMapCalo[(41+ii)*100 + ich + 1];
			int modid = val%100;

			int lrid = (val/100==10) ? 0 : 1;

			if ( lrid==0 ){
				cout << "mid: " << 41+ii << ", ch: " << ich+1 << ", L" << modid << endl;
			}else if ( lrid==1 ){
				cout << "mid: " << 41+ii << ", ch: " << ich+1 << ", R" << modid << endl;
			}

			for (int aa=0; aa<7; aa++){
				for (int bb=0; bb<4; bb++){

					if ( int(H2CaloCoord->GetBinContent(H2CaloCoord->FindBin(aa+0.5,bb+0.5)))==modid ){

						c4[lrid]->cd( aa + 1 + 8*(3 - bb));
						gPad->SetTicks();
						gPad->SetMargin(0.12,0.05,0.12,0.05);
						//TH1D *htmp = (TH1D*)gPad->DrawFrame(500,1,800,10);
						TH1D *htmp = (TH1D*)gPad->DrawFrame(450,1,775,5e4);
						htmp->GetYaxis()->SetRangeUser(0, 1.5*htmp->GetMaximum());
						htmp->GetXaxis()->SetTitle("HV");
						htmp->GetXaxis()->SetTitleSize(0.05);
						htmp->GetXaxis()->SetLabelSize(0.045);
						htmp->GetYaxis()->SetTitle("INT ADC");
						htmp->GetYaxis()->SetTitleSize(0.05);
						htmp->GetYaxis()->SetLabelSize(0.045);

						gHV[ii][ich]->Draw("P");

						TF1 *f1 = new TF1("f1","exp([0]*x + [1])",475,775);
						gHV[ii][ich]->Fit(f1,"R0Q");
						f1->Draw("same");

						f1->SetRange(450,1000);
						f1->SetNpx(500);
						float valX = f1->GetX(intADC_val[aa]);

						TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.9);
						leg->SetBorderSize(0);
						leg->SetTextSize(0.055);
						leg->AddEntry("",Form("IntADC: %4.1f", intADC_val[aa]),"h");
						leg->AddEntry("",Form("log(IntADC): %4.1f", log(intADC_val[aa])),"h");
						leg->AddEntry("",Form("HV: %4.1f", valX),"h");
						leg->Draw();

						break;
					}
				}//bb

			}//aa
		}//ich
	}



}
