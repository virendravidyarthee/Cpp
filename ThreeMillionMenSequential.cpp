/*Throughout the code, the terms SFC and SSC stand for 'Sorted by First Component' and
*'Sorted by Second Component' respectively*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cassert> //Used to check if file is being read


typedef std::map<std::string, std::string> mapOfStrings;

void readFromFile(int &tMen, char* path, mapOfStrings* SFC, mapOfStrings* SSC);
void populateFHG(int tMen, mapOfStrings* SFC, mapOfStrings* SSC, mapOfStrings* dataF, mapOfStrings* dataH, std::map<std::string, int>* dataG);
std::map<int, std::string> computeFinalResult(int tMen, mapOfStrings* dataF, std::map<std::string, int>* dataG, mapOfStrings* dataH);

int main(int argc, char* argv[])
{
	int totalMen = 1;
	mapOfStrings* sortedByFirstComponent;
	mapOfStrings* sortedBySecondComponent;
	mapOfStrings* F;
	mapOfStrings* H;
	std::map<std::string, int>*G;
	std::map<int, std::string> finalResult;



	sortedByFirstComponent = new mapOfStrings;
	sortedBySecondComponent = new mapOfStrings;
	readFromFile(totalMen, argv[1],sortedByFirstComponent,sortedBySecondComponent); // This functions reads from file path and fills the containers sortedBySecondComponent
	F = new mapOfStrings;
	G = new std::map<std::string,int>;
	H = new mapOfStrings;
	populateFHG(totalMen, sortedByFirstComponent,sortedBySecondComponent, F, H, G);
	delete sortedByFirstComponent;
	delete sortedBySecondComponent;
	//F and G are available at this point. Initial phase thus ends here

	finalResult = computeFinalResult(totalMen, F, G, H); //Second phase of Hardy's algo begins here

	delete F;
	delete G;
	delete H;

	std::map<int, std::string>::iterator finalItr;
	finalItr = finalResult.begin();
	for (finalItr; finalItr != finalResult.end(); ++finalItr)
	{
		std::cout << finalItr->first << ": " << finalItr->second << std::endl;
	}
	return 0;
}

void readFromFile(int &tMen, char* path, mapOfStrings* SFC, mapOfStrings* SSC)
{
	std::cout << "Reading from file. Please wait..." << std::endl;
	std::fstream inFile(path);
	
	std::string line, firstComponent, secondComponent;

	std::map<std::string, std::string>::iterator SSC_Iterator, SFC_Iterator;
	assert(inFile.is_open());
	while (std::getline(inFile, line))
	{
		tMen++;
		firstComponent = line.substr(0, line.find(','));
		secondComponent= line.substr(line.find(',') + 1);
		(*SFC)[firstComponent] = secondComponent;
		(*SSC)[secondComponent] = firstComponent;
	}
	inFile.close();
	std::cout << "Done\nComputing data now. Please wait..." << std::endl;
	return;
}

void populateFHG(int tMen, mapOfStrings* SFC, mapOfStrings* SSC, mapOfStrings* dataF, mapOfStrings* dataH, std::map<std::string, int>* dataG)
{
	
	std::map<std::string, std::string>::iterator SFC_Iterator, SSC_Iterator;
	

	//Initial phase begins here(Creating F here)
	SFC_Iterator = SFC->begin();
	SSC_Iterator = SSC->begin();

	while (SFC_Iterator != SFC->end() || SSC_Iterator != SSC->end())
	{
		if (SSC_Iterator->first == SFC_Iterator->first)
		{
			//Add to F, H and advance iteration for both.
			(*dataF)[SFC_Iterator->second] = SSC_Iterator->second;
			(*dataH)[SSC_Iterator->second] = SFC_Iterator->second;
			++SFC_Iterator;
			++SSC_Iterator;
		}
		else if (SSC_Iterator->first > SFC_Iterator->first)
		{
			//Advance first
			++SFC_Iterator;
		}
		else if (SSC_Iterator->first < SFC_Iterator->first)
		{
			//Advance second
			(*dataG)[SSC_Iterator->first] = tMen;
			(*dataG)[SSC_Iterator->second] = tMen - 1;
			++SSC_Iterator;
		}
	}
}

std::map<int, std::string> computeFinalResult(int tMen, mapOfStrings* dataF, std::map<std::string, int>* dataG, mapOfStrings* dataH)
{
	mapOfStrings fPrime, hPrime; //hPrime is an addition to Hardy's algo. Adding this will help create H faster
	std::map<std::string, int> gPrime;
	std::map<std::string, int>::iterator G_Iterator;
	std::map<int, std::string> result;
	mapOfStrings::iterator H_Iterator, F_Iterator;

	int t = 2;
	while (t<tMen)
	{
		F_Iterator = dataF->begin();
		H_Iterator = dataH->begin();
		G_Iterator = dataG->begin();
		while (F_Iterator != dataF->end())
		{
			if (H_Iterator != dataH->end() && F_Iterator->first == H_Iterator->first)
			{
				fPrime[H_Iterator->second] = F_Iterator->second;
				hPrime[F_Iterator->second] = H_Iterator->second;
				++F_Iterator;
				++H_Iterator;
			}
			else if (G_Iterator != dataG->end() && F_Iterator->first == G_Iterator->first)
			{
				gPrime[F_Iterator->second] = G_Iterator->second - t;
				++F_Iterator;
				++G_Iterator;
			}
			else if (H_Iterator != dataH->end() && F_Iterator->first > H_Iterator->first)
			{

				++H_Iterator;
			}
			else if (G_Iterator != dataG->end() && F_Iterator->first > G_Iterator->first)
			{

				++G_Iterator;
			}
		}

		dataH->clear();
		dataH->insert(hPrime.begin(), hPrime.end());
		hPrime.clear();

		dataF->clear();
		dataF->insert(fPrime.begin(), fPrime.end());
		fPrime.clear();


		dataG->insert(gPrime.begin(), gPrime.end());
		gPrime.clear();
		t *= 2;
	}

	std::cout << std::endl << std::endl;
	G_Iterator = dataG->begin();
	for (G_Iterator; G_Iterator != dataG->end(); G_Iterator++)
	{
		result[G_Iterator->second - 1] = G_Iterator->first;
	}

	return result;
}
