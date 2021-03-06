#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>

using namespace std;

#define DATA_PATH1 "/mnt/idata01/data/run-%ddata-%d"
#define DATA_PATH2 "/mnt/idata02/data/run-%ddata-%d"
#define DATA_PATH3 "/mnt/idata03/data/run-%ddata-%d"
#define DATA_PATH4 "/mnt/idata04/data/run-%ddata-%d"
#define DATA_PATH5 "/mnt/idata05/data/run-%ddata-%d"
#define DATA_PATH6 "/mnt/idata06/data/run-%ddata-%d"
#define DATA_PATH7 "/mnt/idata07/data/run-%ddata-%d"

#define ES_MAGIC1 0xaa55f154
#define ES_MAGIC2 0xaa55f15f


#define NCHAN 24

int badModule=0;
int badEvent=0;

struct Sample 
{
    unsigned data[NCHAN];
};

int pulsenum;
std::vector <Sample*> pulse1,pulse2;

typedef std::vector<Sample*>::iterator IT;


static unsigned Crc32_ComputeBuf(unsigned inCrc32, const void *buf, size_t bufLen) 
{
    static const unsigned crcTable[256] = 
	{
	    0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
	    0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
	    0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
	    0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
	    0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
	    0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
	    0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
	    0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	    0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
	    0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
	    0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
	    0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
	    0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
	    0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
	    0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
	    0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	    0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
	    0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
	    0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
	    0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
	    0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
	    0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
	    0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
	    0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	    0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
	    0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
	    0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
	    0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
	    0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
	    0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
	    0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
	    0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	    0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
	    0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
	    0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
	    0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
	    0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
	};
    unsigned crc32;
    unsigned char *byteBuf;
    size_t i;

    /** accumulate crc32 for buffer **/
    crc32 = inCrc32 ^ 0xFFFFFFFF;
    byteBuf = (unsigned char*) buf;
    for (i = 0; i < bufLen; i++) 
    {
	crc32 = (crc32 >> 8) ^ crcTable[(crc32 ^ byteBuf[i]) & 0xFF];
    }
    return crc32 ^ 0xFFFFFFFF;
}



bool is_es1(Sample* es)
{
    return 
	es->data[0] == ES_MAGIC1 &&
	es->data[1] == ES_MAGIC1 &&
	es->data[2] == ES_MAGIC1 &&
	es->data[3] == ES_MAGIC1 &&
	es->data[8] == ES_MAGIC1 &&
	es->data[9] == ES_MAGIC1 &&
	es->data[10] == ES_MAGIC1 &&
	es->data[11] == ES_MAGIC1 &&
	es->data[16] == ES_MAGIC1 &&
	es->data[17] == ES_MAGIC1 &&
	es->data[18] == ES_MAGIC1 &&
	es->data[19] == ES_MAGIC1;
}

bool is_es2(Sample* es)
{
    return 
	es->data[0] == ES_MAGIC2 &&
	es->data[1] == ES_MAGIC2 &&
	es->data[2] == ES_MAGIC2 &&
	es->data[3] == ES_MAGIC2 &&
	es->data[8] == ES_MAGIC2 &&
	es->data[9] == ES_MAGIC2 &&
	es->data[10] == ES_MAGIC2 &&
	es->data[11] == ES_MAGIC2 &&
	es->data[16] == ES_MAGIC2 &&
	es->data[17] == ES_MAGIC2 &&
	es->data[18] == ES_MAGIC2 &&
	es->data[19] == ES_MAGIC2;
}


int processPulse(const char* rawname, Sample* es1,Sample* es2)
{
    unsigned my_value1=0;
    unsigned my_value2=0;
    unsigned *outCrc32_1;
    unsigned *outCrc32_2;
    outCrc32_1=&my_value1;
    outCrc32_2=&my_value2;

    for (IT it = pulse1.begin(); it != pulse1.end(); ++it)
    {
	*outCrc32_1 = Crc32_ComputeBuf(*outCrc32_1,*it, sizeof(Sample));
	delete [] (*it);
    }

  for (IT it = pulse2.begin(); it != pulse2.end(); ++it)
    {
	*outCrc32_2 = Crc32_ComputeBuf(*outCrc32_2,*it, sizeof(Sample));
	delete [] (*it);
    }

   //  fprintf(stdout, "%12d %08x %08x %s crc32:%08x\n", 
   // 	    es1->data[4], es1->data[5], es1->data[7], 
   // 	    my_value1==es1->data[5]&& my_value1==es1->data[7]? "GOOD": "BAD", my_value1);

   // fprintf(stdout, "%12d %08x %08x %s crc32:%08x\n", 
   // 	    es2->data[4], es2->data[5], es2->data[7], 
   // 	    my_value2==es2->data[5]&& my_value2==es2->data[7]? "GOOD": "BAD", my_value2);

   if(!(my_value1==es1->data[5]&& my_value1==es1->data[7]) && es1->data[4] > 5 ) 
   {
       pulse1.clear();
       pulse2.clear();
       return -1;
   }
   if(!(my_value2==es2->data[5]&& my_value2==es2->data[7]) && es1->data[4] > 5)
   {
       pulse1.clear();
       pulse2.clear();
       return -2;
   }

    pulse1.clear();
    pulse2.clear();
    return 0;
}
int process(const char* rawname,int run,int  daq)
{
    int status=0;
    FILE* fp_in = fopen(rawname, "r");
    if (fp_in == 0)
    {
	perror(rawname); exit(1);
    }
    Sample* sample1 = new Sample;
    Sample* sample2 = new Sample;

    enum STATE { LOOK_FIRST_ES, LOOK_SECOND_ES } state = LOOK_FIRST_ES;

    while(fread(sample1->data, sizeof(unsigned), NCHAN, fp_in) == NCHAN && fread(sample2->data, sizeof(unsigned), NCHAN, fp_in) == NCHAN)
    {
	switch(state)
	{
	case LOOK_FIRST_ES:
	    if (is_es1(sample1))
	    {
		state = LOOK_SECOND_ES;
		break;
	    }
	case LOOK_SECOND_ES:
	    if (is_es1(sample1) && is_es2(sample2))
	    {
		status=processPulse(rawname, sample1,sample2);
		if(status == -1)
		{
		    cout << "BAD Checksum for  Module-1 of run: "<<run<<" DAQ: "<< daq<<" Event :"<<sample1->data[4] <<endl;
		    badModule=1;
		    badEvent=sample1->data[4];
		    return -1;
		}
		if(status == -2)
		{
		    cout << "BAD Checksum for  Module-2 of run: "<<run<<" DAQ: "<< daq<<" Event :"<<sample1->data[4] <<endl;
		    badModule=2;
		    badEvent=sample1->data[4];
		    return -1;
		}

	    }
	    else
	    {
		pulse1.push_back(sample1);
		pulse2.push_back(sample2);
		sample1 = new Sample;
		sample2 = new Sample;
	    }
	}
    }
    fclose(fp_in);
    return 0;
}
int n3HeChecksum(int runNumber)
{
    int daq;
    int daq_status=0;
    int run_status=0;
    bool missing_file=false;
    string dataPath;

    if(runNumber > 0 && runNumber < 23662)
    	dataPath=DATA_PATH1;
    else if(runNumber > 23661 && runNumber <31840)
    	dataPath=DATA_PATH2;
    else if(runNumber > 31839 && runNumber <40704)
    	dataPath=DATA_PATH3;
    else if(runNumber > 40703 && runNumber < 44386)
    	dataPath=DATA_PATH4;
    else if(runNumber > 44385 && runNumber < 46000)
    	dataPath=DATA_PATH5;
    else if(runNumber > 45999 && runNumber< 54041)
    	dataPath=DATA_PATH6;
    else if(runNumber > 54040 )
    	dataPath=DATA_PATH7;

    cout << "Analyzing run number: "<<runNumber<<flush;

    for( daq=21; daq<25; daq++)
    {
	cout << " . " <<flush;

	char *data_file=new char[200];
	sprintf(data_file,dataPath.c_str(),runNumber, daq);
	ifstream runData(data_file);

	if (!runData) 
	{
	    cout << "File does not exist for run: "<<runNumber<<" Module: "<< daq<<endl;
	    missing_file=true;
	    continue;
	}
	else
	{
	    daq_status=process(data_file,runNumber, daq);
	    if(daq_status<0)
	    {
		ofstream Checksum("/mnt/idata05/summary/BadChecksum.txt",ofstream::app);
		    if(Checksum)
		    {
			Checksum<<runNumber<<"\t\t\t\t"<<daq<<"\t\t\t\t"<<badModule<<"\t\t\t\t"<<badEvent<<endl;
			Checksum.close();
		    }
		    else
		    {
			cout << "Unable to Create BadChecksum recort file" <<endl;
			return -1;
		    }
	    }
	    run_status=run_status+daq_status;
	}
	runData.close();
	delete[] data_file;
    }

    if(run_status!=0)
	cout <<"\t\t BAD" <<endl;
    else if(run_status==0 && !missing_file)
	cout <<"\t\t GOOD" <<endl;
    
    return 0;
}

void signalHandler( int signum )
{
    cout << "\n Interrupt signal (" << signum << ") received.\n";
    exit(signum);  
}

int main(int argc, char *argv[])
{
    int start_run=0;
    int last_run=0;
    bool quit_now=false;
    int status=0;

    cout<<"Enter the run range separated by space"<<endl;
    cin>>start_run;
    cin>>last_run;
    cout<<"Requested run range: "<<start_run<<" to "<<last_run<<endl;
    
    ifstream inFile("/mnt/idata05/summary/runListUD.txt");
    if(!inFile)
    {
	cout<<"Could NOT locate data file"<<endl;
	return -1;
    }

    int run;
    int error_code;
    string error;
    int index=0;
    int n3he_first=17400; //First available n3He run number  
    int n3he_last=62000;  //Last available n3He run number 

    if(start_run==0)
    {
	cout << "Syntax: RunStatus(lower_range,upper_range)" <<endl;
	cout << "Available Run range "<<n3he_first<<" to "<<n3he_last<<endl;

	return -1;
    }

    if(start_run!=0 && last_run==0)
	last_run=start_run;

    if(start_run<n3he_first || start_run > n3he_last || last_run<n3he_first || last_run > n3he_last)
    {
	cout<<"Run Data for run number below "<<n3he_first<<" or above "<<n3he_last<<" is NOT available"<<endl;
	return -1;
    }
    
    signal(SIGINT, signalHandler);  //Handle Ctrl+C signal
    while(!inFile.eof())
    {
	inFile>>run>>error_code>>error;

	if(run>=start_run && run<=last_run && error_code>=0)
	{	    
	    status=n3HeChecksum(run);
	    index++;
	}
	if(run>=last_run)
	    break;
	if(status<0)
	    break;
    }

    if(index>100)
	cout<<"Requested run range was: "<<start_run<<" to "<<last_run<<endl;

    inFile.close();

    return 0;
}
