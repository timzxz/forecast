#include "use.hpp"
using namespace Tree;
void pretitle(string file)
{
    int cnt(0),ncnt(0),head(0),state(0);
    string tmp;
    ifstream in(file,ios::binary);
    ofstream out("pdata/ptitle.pdt");
    if(in.is_open())
    {
        cout<<"********** PRE "<<file<<" START **********"<<endl;
        while(!in.eof())
        {
            in>>tmp;
            if(state==2)
            {
                head=tmp.find("\"title_url\"");
                out<<(tmp.substr(head-7,5))<<'\n';
                state=0;
            }
            if(state==0)
            {
                head=tmp.find("\"_id\":");
                if(head!=string::npos)
                {
                    ncnt++;
                    out<<(tmp.substr(head+7,10))<<' ';
                    state=1;
                }
            }
            if(state==1)
            {
                head=tmp.find("\"title_time\"");
                if(head!=string::npos)
                {
                    out<<(tmp.substr(head+14,11))<<'-';
                    state=2;
                }
            }
        }
        in.close();
        cout<<"********** PRE-NEWS OVER. NEWS COUNT : "<<ncnt<<endl;
    }
    out.close();
};
void precomment(string file)
{
    int ncnt(0),head(0),state(0);
    string tmp,cid,pid,tid;
    ifstream in(file,ios::binary);
    ofstream out("pdata/pcomment.pdt");
    shared_ptr<cmt> tmpp,prtp;
    if(in.is_open())
    {
        cout<<"********** PRE "<<file<<" START **********"<<endl;
        while(!in.eof())
        {
            in>>tmp;
            if(state==2)
            {
                head=tmp.find("\"}");
                out<<tmp.substr(head-5,5)<<'\n';
                state=0;
            }
            if(state==0)
            {
                head=tmp.find("\"_id\":");
                if(head!=string::npos)
                {
                    ncnt++;
                    out<<tmp.substr(head+7,19)<<' ';//cid
                    out<<tmp.substr(head+40,10)<<' ';//tid
                    head=tmp.find("\"pid\":null");//pid
                    if(head!=string::npos)//NOT reply
                        out<<"null\n";
                    else//IS reply
                    {
                        head=tmp.find("\"pid\":");
                        out<<tmp.substr(head+7,19)<<'\n';
                    }
                    state=1;
                }
            }
            if(state==1)
            {
                head=tmp.find("\"time\"");
                if(head!=string::npos)
                {
                    out<<tmp.substr(head+8,10)<<'-';
                    state=2;
                }
            }
        }
        in.close();
        cout<<"********** PRE-COMMENTS OVER. COMMENTS COUNT : "<<ncnt<<endl;
    }
    out.close();
};
void test()
{
    ifstream in("pdata/pcomment.pdt");
    string tmp;
    int cnt(0);
    if(in.is_open())
    {
        cout<<"test cmts"<<endl;
        while(!in.eof())
        {
            in>>tmp;
            cnt++;
            if((cnt>9900)&&(cnt<10000))
                cout<<tmp<<' ';
            if(cnt>10000)
                break;
        }
        in.close();
    }
}
//void test()
//{
//    ifstream in("json/commentitem.json");
//    string tmp;
//    int cnt(0);
//    while(++cnt<10)
//    {
//        in>>tmp;
//        cout<<tmp<<endl;
//    }
//}
int main()
{
    auto preadnews=bind(pretitle,string("json/titleitem.json"));
    auto preadcmts=bind(precomment,string("json/commentitem.json"));
    map<string,function<void(void)>> preread= {{"news",preadnews},{"comments",preadcmts}};
    cout<<"READ-ORIGINAL AND PRE-DISPOSE BEGIN"<<endl;
    preread["news"]();
    preread["comments"]();
    test();
    cout<<"READ-ORIGINAL AND PRE-DISPOSE END"<<endl;
}
