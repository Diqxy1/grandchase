#include "stdafx.h"
#include ".\kgcperformancecounter.h"


FILE* KGCPerformanceCounter::fp;
int KGCPerformanceCounter::frequent;

std::map<std::string,KGCPerformanceCounter*> KGCPerformanceCounter::countermap;
KGCPerformanceCounter* KGCPerformanceCounter::current;
std::stringstream KGCPerformanceCounter::memo;
std::stringstream KGCPerformanceCounter::title;
bool KGCPerformanceCounter::ms_bWriteTitle = false;
std::set<std::string> KGCPerformanceCounter::titlelist;

KGCPerformanceCounter::KGCPerformanceCounter(KGCPerformanceCounter* p,std::string key)
{
	parent = p;
	total.QuadPart=0;
	this->key = key;
	calls =0;
}

KGCPerformanceCounter::~KGCPerformanceCounter(void)
{
}

void KGCPerformanceCounter::Write()
{
	fprintf(fp,"%d\t",total.QuadPart/frequent);
	total.QuadPart = 0;

	std::map<std::string,KGCPerformanceCounter*>::iterator i= child.begin();
	while(i!=child.end())
	{
		i->second->Write();
		i++;
	}
	calls=0;
}


void KGCPerformanceCounter::Begin(std::string key)
{
	KGCPerformanceCounter *pc;
	if(current==NULL)
	{
		if(countermap.find(key)==countermap.end())
		{
			pc = new KGCPerformanceCounter(NULL,key);
			countermap.insert(std::map<std::string,KGCPerformanceCounter*>::value_type(key,pc));
		}
		else
		{
			pc = countermap[key];
		}
	}
	else
	{
		if(current->child.find(key)==current->child.end())
		{
			pc = new KGCPerformanceCounter(current,key);
			current->child.insert(std::map<std::string,KGCPerformanceCounter*>::value_type(key,pc));
		}
		else
		{
			pc = current->child[key];
		}
	}
    if( titlelist.find( key ) == titlelist.end() ) {
        titlelist.insert( key );
        ms_bWriteTitle = true;
    }

	current = pc;
	current->calls++;
	QueryPerformanceCounter(&current->pre);
}

void KGCPerformanceCounter::End( int iValue_ /*= -1 */ )
{
    if( iValue_ < 0 ) {
        QueryPerformanceCounter(&current->now);
        current->total.QuadPart += current->now.QuadPart - current->pre.QuadPart;
    }
    else {
        current->total.QuadPart += iValue_;
    }
    current=current->parent;
}

void KGCPerformanceCounter::WriteCounter()
{
	static int totalcount = 1;
	totalcount++;

	if(totalcount>frequent)
	{
        if( ms_bWriteTitle ) {
            WriteAllTitle();
            ms_bWriteTitle = false;
        }

		std::map<std::string,KGCPerformanceCounter*>::iterator i= countermap.begin();
		while(i!=countermap.end())
        {
			i->second->Write();
            i++;
        }

        if( countermap.empty() == false ) {
            if( memo.str().empty() == false ){
                fprintf(fp, "%s", memo.str().c_str() );
                memo.str("");
            }

            fprintf(fp,"\n");
        }
		totalcount = 1;
	}
    fflush( fp );
}


void KGCPerformanceCounter::BeginCounter( std::string filename,int f )
{
	fp = fopen(filename.c_str(),"wt");
	frequent=f;
}

void KGCPerformanceCounter::EndCounter()
{
	fclose(fp);
}

void KGCPerformanceCounter::AddMemo( std::string str )
{
    memo<<str<<"    ";
}

void KGCPerformanceCounter::WriteAllTitle()
{
    std::map<std::string,KGCPerformanceCounter*>::iterator i= countermap.begin();
    while(i!=countermap.end())
    {
        i->second->WriteTitle();
        ++i;
    }
    fprintf( fp, "\n" );
}

void KGCPerformanceCounter::WriteTitle()
{
    fprintf( fp, "%s\t", key.c_str() );

    std::map<std::string,KGCPerformanceCounter*>::iterator i= child.begin();
    while(i!=child.end())
    {
        i->second->WriteTitle();
        ++i;
    }
}

