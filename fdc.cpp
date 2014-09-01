// mysql.cpp : ¶¨Òå¿ØÖÆÌ¨Ó¦ÓÃ³ÌÐòµÄÈë¿Úµã¡£
//

//#include "stdafx.h"
//#include <winsock.h>
#include <iostream>
#include <sstream>
#include <string>
#include <mysql.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
//#include <hash_map>
//using stdext::hash_map;
using namespace std;
int jishuqi=0;
MYSQL mydata;
int a=0;


#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"libmysql.lib")

#define STEPBYSTEP

int Tagging(string *tagid,string *tagname,int tagcount);
int Taggable(string *taggable_id,string *taggable_type,int taggingcount,string key_tag_id,string keytag,string *tagid,string *tagname,int tagcount);
int connectmysql(MYSQL &mydata);
int FDC(string key_tag_id,string keytag,string **descript,string *tags,int tagcount,int taggingcount);
int FC(string description,string key_tag_id,string keytag,string *tags,float *frequentness,int tagcount,float *common,float *distance);
int D(string description,string key_tag_id,string keytag ,string  *tags,float *distance,int tagcount,int *number,string * tagid);
int shortest(int keyi,int coi,int *coordinate,float distance,int keynumber,int conumber);
int qksort(string *qksorttags,float *qksortfdc,int low,int high);
//int participle(int &descriptioncount,int &dictionarycount,vector <string> dictionary,hash_map <string,int> dictionaryid,hash_map <string,int> dictionarynumber);


int main(int argc, char* argv[])
{
              //±Ø±¸µÄÒ»¸öÊý¾Ý½á¹¹
  
  connectmysql(mydata);
//  system("pause");
  string sqlstr;
  //hash_map <string> dictionary;
//  hash_map <string,int> dictionaryid;
//  hash_map <string,int> dictionarynumber;
//  dictionary.reserve(10000);
  //dictionarynumber.reserve(10000);
//  int descriptioncount=0;
//  int dictionarycount=0;
//  participle(descriptioncount,dictionarycount,dictionary,dictionaryid,dictionarynumber);


                                                //±éÀútags    
  sqlstr = "SELECT id,name FROM tags WHERE used_count>5 ORDER BY used_count DESC";
  MYSQL_RES *result = NULL;

  int tagcount=0;
  if (0 == mysql_query(&mydata, sqlstr.c_str())) {
    //Ò»´ÎÐÔÈ¡µÃÊý¾Ý¼¯
    result = mysql_store_result(&mydata);
    
    //mysql_refresh(&mydata, REFRESH_STATUS);
    //È¡µÃ²¢´òÓ¡ÐÐÊý
    int rowcount = mysql_num_rows(result);
    string *tagid = new string[rowcount];
    string *tagname = new string[rowcount];
    //´òÓ¡¸÷ÐÐ
    MYSQL_ROW row = NULL;
    row = mysql_fetch_row(result);
    int tagcount=0;               //¼ÆÊýÆ÷
    while (NULL != row) {
      string name_length=row[1];
        tagid[tagcount]=row[0];
        tagname[tagcount]=row[1];
//      cout<<tagid[tagcount]<<endl<<tagname[tagcount]<<endl;
//      if(tagcount==100)
//          break;
        tagcount++;
      row = mysql_fetch_row(result);
    }
    Tagging(tagid,tagname,tagcount);
    delete [] tagid;
    delete [] tagname;
    mysql_free_result(result);
  } 
  else {
    cout << "mysql_query() select data failed   0 " << endl;
//    system("pause");
  }
//  cout<<endl<<endl<<endl<<jishuqi<<endl<<endl;
//  mysql_close(&mydata);
//  system("pause");
//  dictionary.clear();
  //dictionarynumber.clear();
  return 0;
}

/*
int participle(int &descriptioncount,int &dictionarycount,vector <string> dictionary,hash_map <string,int> dictionaryid,hash_map <string,int> dictionarynumber)
{
  string sqlstr = "SELECT id,description FROM interviews";
  MYSQL_RES *result = NULL;
  if(0 == mysql_query(&mydata,sqlstr.c_str()))
  {
    result = mysql_store_result(&mydata);
    int rowcount = mysql_num_rows(result);
    vector <string> id;
    vector <string> description;
    id.reserve(rowcount);
    description.reserve(rowcount);
    MYSQL_ROW row=NULL;
    row = mysql_fetch_row(result);
    string test2="test2.txt";
    string test2result="test2_result.txt";
    while (NULL!=row)
    {
      if(NULL!=row[1])
      {
      id.push_back(row[0]);
      cout<<"id:"<<row[0]<<endl;
      description.push_back(row[1]);
      fstream f(test2.c_str(),ios::out);
      f<<description[descriptioncount];
      f.close();
      system("test_fenci.exe");
      f.open("test2_result.txt",ios::in);
      vector <string> descriptionparticiple;
      descriptionparticiple.reserve(10000);
      int i=0;
      string d;
      while((f>>d)&&(!d.empty()))
      {
        descriptionparticiple.push_back(d);
        if(!(descriptionparticiple[i][descriptionparticiple[i].size()-1]=='w'))
        {
        if(dictionarynumber[descriptionparticiple[i]]==0)
        {
          dictionaryid[descriptionparticiple[i]]=dictionarycount;
          dictionary.push_back(descriptionparticiple[i]);
          dictionarycount++;
        }
        dictionarynumber[descriptionparticiple[i]]++;
        }
        i++;
      }
      cout<<endl<<endl<<dictionary.size()<<endl;
      f.close();
      descriptionparticiple.clear();
      cout<<descriptioncount<<"\n\n\n";
      descriptioncount++;}
      row = mysql_fetch_row(result);
    }
    for(int j=0;j<dictionary.size();j++)
    {
      stringstream stream;
      string i_s;
      string participletxt="participle.txt";
      fstream f(participletxt.c_str(),ios::out&&ios::app);
//      cout<<j<<"\t"<<dictionary[j]<<"\t"<<dictionarynumber[dictionary[j]]<<endl;
      sqlstr = "INSERT INTO dictionarys VALUES(\"";
      stream<<j+1;
      i_s=stream.str();
      f<<i_s;
      sqlstr+=i_s;
      stream.str("");
      sqlstr+="\",\"";
      f<<dictionary[j];
      sqlstr+=dictionary[j];
      sqlstr+="\",\"";
      stream<<dictionarynumber[dictionary[j]];
      i_s=stream.str();
      f<<i_s;
      f<<",,,";
      sqlstr+=i_s;
      stream.str("");
      sqlstr+="\")";
      if (0 == mysql_query(&mydata, sqlstr.c_str())) {
      cout << "mysql_query() insert data succeed in"<<"\t"<<j << endl;
      } else {
      cout << "mysql_query() insert data failed in" <<"\t"<<j<< endl;
      cout << sqlstr<<endl;
      
      system("pause");
      mysql_close(&mydata);
      //return -1;
      }
      f.close();
    }
    
    id.clear();
    description.clear();
    mysql_free_result(result);
    cout<<"\n\n\n\n"<<"OK"<<endl;
  }
  else
  {
    cout << "mysql_query() select data failed  participle " << endl;
    system("pause");
  }
  return 0;
}
*/










/*int participle(int &descriptioncount,vector <string> dictionary,vector <int> dictionarynumber)
{
  string sqlstr = "SELECT id,description FROM interviews";
  MYSQL_RES *result = NULL;
  if(0 == mysql_query(&mydata,sqlstr.c_str()))
  {
    result = mysql_store_result(&mydata);
    int rowcount = mysql_num_rows(result);
    vector <string> id;
    vector <string> description;
    id.reserve(rowcount);
    description.reserve(rowcount);
    MYSQL_ROW row=NULL;
    row = mysql_fetch_row(result);
    string test2="test2.txt";
    string test2result="test2_result.txt";
    while (NULL != row)
    {
      id.push_back(row[0]);
      description.push_back(row[1]);
      fstream f(test2.c_str(),ios::out);
      f<<description[descriptioncount];
//      cout<<description[descriptioncount]<<endl;
      f.close();
      system("test_fenci.exe");
//      system("pause");
      f.open("test2_result.txt",ios::in);
      vector <string> descriptionparticiple;
      vector <bool> dictionaryempty;
      descriptionparticiple.reserve(10000);
      dictionaryempty.reserve(10000);
      int i=0;
      string d;
      while(f>>d)
      {
        descriptionparticiple.push_back(d);
//        cout<<descriptionparticiple[i]<<"\t";
        int dictionaryi=0;
        int j=0;
        while(j<dictionary.size())
        {
          dictionaryempty.push_back(true);
          j++;
        }
        while(dictionaryi<dictionary.size()&&dictionary[dictionaryi]!=descriptionparticiple[i])
        {
          dictionaryi++;
        }
        if(dictionaryi<dictionary.size()&&dictionaryempty[dictionaryi]==true)
        {
          dictionarynumber[dictionaryi]++;
          dictionaryempty[dictionaryi]=false;
        }
        if(dictionaryi==dictionary.size())
        {
          dictionary.push_back(descriptionparticiple[i]);
          dictionarynumber.push_back(1);
          dictionaryempty.push_back(false);
        }
        i++;
      }
      cout<<endl<<endl<<dictionary.size()<<endl<<endl;
      f.close();
      dictionaryempty.clear();
      descriptionparticiple.clear();
      descriptioncount++;
      row = mysql_fetch_row(result);
    }
    id.clear();
    description.clear();
    mysql_free_result(result);
  }
  else
  {
    cout << "mysql_query() select data failed  participle " << endl;
    system("pause");
  }
  return 0;
}
*/













int Tagging(string *tagid,string *tagname,int tagcount)
{ 
  //for(int i=0;i<tagcount;i++)
  string keytag;
  while(cin>>keytag)
  {
    MYSQL_RES *result = NULL;
    string sqlstr = "SELECT id FROM tags where name=\"";
    sqlstr+=keytag;
    sqlstr+="\"";
    result = NULL;
    string key_tag_id;
    if (0 == mysql_query(&mydata, sqlstr.c_str())) {
      result = mysql_store_result(&mydata);
      MYSQL_ROW row = NULL;
      row = mysql_fetch_row(result);
      if (NULL != row) 
        key_tag_id=row[0];
    } else { 
      cout << "mysql_query() select data failed    qqq" << endl;
    }
    if (key_tag_id.empty( ))
    {
      cout <<"没有此标签"<<endl;
      continue;
    }
    sqlstr = "SELECT taggable_id,taggable_type FROM taggings where tag_id=";
    sqlstr+=key_tag_id;
    result = NULL;
    if (0 == mysql_query(&mydata, sqlstr.c_str())) {
      //Ò»´ÎÐÔÈ¡µÃÊý¾Ý¼¯
      result = mysql_store_result(&mydata);
      //È¡µÃ²¢´òÓ¡ÐÐÊý
      int rowcount = mysql_num_rows(result);
      string *taggable_id=new string [rowcount];
      string *taggable_type=new string [rowcount];
      //´òÓ¡¸÷ÐÐ
      MYSQL_ROW row = NULL;
      row = mysql_fetch_row(result);
      int taggingcount=0;               //¼ÆÊýÆ÷
      while (NULL != row) {
        taggable_id[taggingcount]=row[0];
        taggable_type[taggingcount]=row[1];
        row = mysql_fetch_row(result);  
        taggingcount++;
//        cout<<taggable_id[taggingcount]<<endl;
      }
      Taggable(taggable_id,taggable_type,taggingcount,key_tag_id,keytag,tagid,tagname,tagcount);
      if(result!=NULL) mysql_free_result(result);
      delete [] taggable_id;
      delete [] taggable_type;
    } else { 
      cout << "mysql_query() select data failed    1" << endl;
//        system("pause");
    }
    /*
    for(int i=0;i<tagcount;i++)
    {
      delete [] fdc[i];
      delete [] copytags[i];
    }
    delete [] fdc;
    delete [] copytags;
    delete [] relationcount;*/
  }
  return 0;
}

                                                    //ÕÒµ½ÔÚinterviewsÖÐ¶ÔÓ¦µÄ±¨µÀ

int Taggable(string *taggable_id,string *taggable_type,int taggingcount,string key_tag_id,string keytag,string *tagid,string *tagname,int tagcount)
{ 

  
//  string text2="test2.txt";
//  string text2_result;
//  ofstream fout(text2.c_str(),ios::trunc);
//  cout<<taggingcount<<endl;
  
        int bbb=0;
  string **description = new string *[taggingcount];
  for (int i=0;i<taggingcount;i++)
  {
    description[i]=new string [10000];
  }
  for (int i=0;i<taggingcount;i++)
  {
    MYSQL_RES *result = NULL;
    if(taggable_type[i]=="StartupFix")
    {
      string sqlstr = "SELECT description FROM startup_fixes where id=";
      sqlstr+=taggable_id[i];
      if (0 == mysql_query(&mydata, sqlstr.c_str())) {
        //description[i]=new string [1];
        //Ò»´ÎÐÔÈ¡µÃÊý¾Ý¼¯
        result = mysql_use_result(&mydata);
        //È¡µÃ²¢´òÓ¡ÐÐÊý
        int rowcount = mysql_num_rows(result);
        //´òÓ¡¸÷ÐÐ
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row(result);
        if (NULL != row) {
          description[i][0]=row[0];           
          jishuqi++;
//          row = mysql_fetch_row(result);
          bbb++;
//          ostringstream oss;
//          oss<<bbb;
//          text2_result=oss.str();
//          text2_result+=".txt";
//          cout<<sqlstr<<endl;
          //cout<<taggingcount<<endl;
//          cout<<text2_result<<endl;
//          cout<<description[i][0];
//          fstream f(text2.c_str(),ios::out);
//          f<<description[i][0];
//          f.close();
//          system("test_fenci.exe");
//          rename("test2_result.txt",text2_result.c_str());
//          system("pause");
        }
      } else {
        cout << "mysql_query() select data failed   9" << endl;
//        system("pause");
      }
      if(result!=NULL) mysql_free_result(result);
    }
    if(taggable_type[i]=="Startup")
    {
      string sqlstr = "SELECT interview_id FROM startup_interviews WHERE startup_id=";
      sqlstr+=taggable_id[i];
//      cout << sqlstr<<endl;
      if (0 == mysql_query(&mydata, sqlstr.c_str())) {
        result = mysql_store_result(&mydata);
        int rowcount = mysql_num_rows(result);
        //description[i]=new string[rowcount];
        string *interviewid = new string[rowcount];
        //´òÓ¡¸÷ÐÐ
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row(result);
        int interviewcount=0;               //¼ÆÊýÆ÷
        while (NULL != row) {
          MYSQL_RES *res = NULL;
          interviewid[interviewcount]=row[0];
          sqlstr="SELECT description FROM interviews WHERE id=";
          sqlstr+=interviewid[interviewcount];
          sqlstr+=" and description is not NULL";
          if (0 == mysql_query(&mydata, sqlstr.c_str()))
          {
            res = mysql_use_result(&mydata);
            MYSQL_ROW r = NULL; 
            r = mysql_fetch_row(res);
            if (NULL != r)
            {
            description[i][interviewcount] = r[0];
            jishuqi++;
            //cout<<description[i][interviewcount]<<endl;
            if(!description[i][interviewcount].empty())
              bbb++;
            }
          }
          mysql_free_result(res);
          row = mysql_fetch_row(result);
          interviewcount++;
        }
        delete [] interviewid;
      } else {
        cout << "mysql_query() select data failed   10" << endl;
//        system("pause");
      }
      if(result!=NULL) mysql_free_result(result);
    }
  }

//  cout<<bbb<<endl<<endl<<endl<<endl<<endl;
  FDC(key_tag_id,keytag,description,tagname,tagcount,taggingcount);
  for(int i=0;i<taggingcount;i++)
  {
    delete [] description[i];
  }
  delete [] description;
  return 0;
}



int FDC(string key_tag_id,string keytag,string **descript,string *tags,int tagcount,int taggingcount)
{
  int descriptioncount = 0;
/*  for(int i=0;i<rowcount;i++)
  {
    if(!description[i].empty())
    {
      description[descriptioncount]=description[i];
      descriptioncount++;
    }
  }
*/
  string *description=new string [100000];
  for(int i=0;i<taggingcount;i++)
  {
    for(int j=0;j<10000;j++)
    {
      if(!descript[i][j].empty())
      {
        description[descriptioncount]=descript[i][j];
        //cout<<description[descriptioncount];
        descriptioncount++;
      } 
    }
  }
//  cout<<descriptioncount<<endl;
//  system("pause");
  int relationcount=0;
  float *fdc=new float [tagcount];
  float **frequentness = new float *[descriptioncount];
  float **distance = new float *[descriptioncount];
  float **common = new float *[descriptioncount];
  for(int i=0;i<descriptioncount;i++)
  {
    frequentness[i]=new float [tagcount];
    distance[i]=new float [tagcount];
    common[i]=new float [tagcount];
    for (int j=0;j<tagcount;j++)
    {
      frequentness[i][j]=0;
      distance[i][j]=10000;
      common[i][j]=0;
    }
  }
  for(int i=0;i<descriptioncount;i++)
  {
    FC(description[i],key_tag_id,keytag,tags,frequentness[i],tagcount,common[i],distance[i]);
  }
  string *copytags=new string [tagcount];
  for(int j=0;j<tagcount;j++)
  {
    fdc[j]=0;
    float fr=0,di=0,co=0;
    for(int i=0;i<descriptioncount;i++)
    {
      fr+=frequentness[i][j];
/*      if(distance[i][j]!=10000)
      {
        if(distance[i][j]==0)
        {
          di+=1;
        }
        else 
        {
          di+=1/distance[i][j];
        }
      }
*/
      if (common[i][j]==1)
      {
        co+=1;
      }

    }
    if (descriptioncount)
    {
      fr=fr/descriptioncount;
//      di=di*descriptioncount;
      co=co/descriptioncount;
    }
//    cout<<fr<<"\t"<<di<<"\t"<<co<<endl;
    fdc[j]=fr*co;

    if(fdc[j])
    {
      copytags[relationcount]=tags[j];
      fdc[relationcount]=fdc[j];
      relationcount++;
    }
  }
  qksort(copytags,fdc,0,relationcount-1);
  for(int i=relationcount-1 ,j = 0;j < 30;i--)
  {
    if(keytag!=copytags[i])
    {
      cout<<keytag<<"\t"<<copytags[i]<<"\t"<<fdc[i]<<endl;
    }
    j++;
  }

  cout<<endl<<endl<<endl<<endl<<endl;
  for(int i=0;i<descriptioncount;i++)
  {
    delete [] frequentness[i];
    delete [] distance[i];
    delete [] common[i];
  }
  delete [] copytags;
  delete [] frequentness;
  delete [] distance;
  delete [] common;
  return 0;
}

int FC(string description,string key_tag_id,string keytag,string *tags,float *frequentness,int tagcount,float *common,float *distance)
{
  float sum=0;
  int *number = new int [tagcount];
  string surplusstr;
  for (int i=0;i<tagcount;i++)
  {
    surplusstr = description;
    int start=0;
    number[i]=0;
    while (start<description.length())
    {
      if(surplusstr.find(tags[i],start)!=-1)
      {
        common[i]=1;
        number[i]++;
        start=surplusstr.find(tags[i],start)+tags[i].length();
        surplusstr.assign(description,start,description.length());
      }
      else
      {
        start=description.length();
      }
    }
    sum+=number[i];
  }
  for(int i=0;i<tagcount;i++)
  {
    if(sum!=0)
    {
      frequentness[i]=number[i]/sum;
    }
    //cout<<tags[keytagi]<<"\t"<<number[keytagi]<<"\t"<<sum<<endl;
  }
  //D(description,keytagi,keytag,tags,distance,tagcount,number);
  delete [] number;
  return 0;
}

int D(string description ,string key_tag_id,string keytag ,string  *tags,float *distance,int tagcount,int *number,string * tagid)
{
  string surplusstr;
  for(int i=0;i<tagcount;i++)
  {
    surplusstr=description;
    int *coordinate=new int [number[i]];
    int start=0;
    int j=0;
    while(j<number[i])
    {
      if(surplusstr.find(tags[i],start)!=-1)
      {
        coordinate[j]=surplusstr.find(tags[i],start);
        start=surplusstr.find(tags[i],start)+tags[i].length();
        surplusstr.assign(description,start,description.length());
      }
      j++;
    }
    distance[i]=10000;
    if(tagid[i]!=key_tag_id)
    {
      shortest(0,0,coordinate,distance[i],0,number[i]);
    }
    delete [] coordinate;
  }
  return 0;
}


int shortest(int keyi,int coi,int *coordinate,float distance,int keynumber,int conumber)
{
  if(keyi<keynumber&&coi<conumber)
  {
    if(coordinate[keyi]<coordinate[coi])
    {
      if(distance>(coordinate[coi]-coordinate[keyi]))
      {
        distance=(coordinate[coi]-coordinate[keyi]);
      }
      keyi++;
      shortest(keyi,coi,coordinate,distance,keynumber,conumber);
    }
    else
    {
      if(distance>(coordinate[keyi]-coordinate[coi]))
      {
        distance=(coordinate[keyi]-coordinate[coi]);
      }
      coi++;
      shortest(keyi,coi,coordinate,distance,keynumber,conumber);
    }
  }
  else
  {
    return 0;
  }
  return 0;
}


int connectmysql(MYSQL &mydata)
{
  //³õÊ¼»¯Êý¾Ý¿â
  if (0 == mysql_library_init(0, NULL, NULL)) {
  } else {
    cout << "mysql_library_init() failed" << endl;
    return -1;
  }
  //³õÊ¼»¯Êý¾Ý½á¹¹
  if (NULL != mysql_init(&mydata)) {
  } else {
    cout << "mysql_init() failed" << endl;
    return -1;
  }
  //ÔÚÁ¬½ÓÊý¾Ý¿âÖ®Ç°£¬ÉèÖÃ¶îÍâµÄÁ¬½ÓÑ¡Ïî
  //¿ÉÒÔÉèÖÃµÄÑ¡ÏîºÜ¶à£¬ÕâÀïÉèÖÃ×Ö·û¼¯£¬·ñÔòÎÞ·¨´¦ÀíÖÐÎÄ
  if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "UTF8")) {
  } else {
    cout << "mysql_options() failed" << endl;
    return -1;
  }
  //Á¬½ÓÊý¾Ý¿â
  if (NULL!= mysql_real_connect(&mydata, "localhost", "root", "mysql", "startups_develop",3306, NULL, 0))
      //ÕâÀïµÄµØÖ·£¬ÓÃ»§Ãû£¬ÃÜÂë£¬¶Ë¿Ú¿ÉÒÔ¸ù¾Ý×Ô¼º±¾µØµÄÇé¿ö¸ü¸Ä
      {
  } else {
    cout << "mysql_real_connect() failed" << endl;
    return -1;
  }
  return 0;
}


int qksort(string *qksorttags,float *qksortfdc,int low,int high)
{
  if (low<high)
  {
    int first=low;
    int last=high;
    float key=qksortfdc[first];
    string keytag=qksorttags[first];
    while(first<last)
    {
    while(first<last&&qksortfdc[last]>=key)
    --last;
    qksortfdc[first]=qksortfdc[last];
    qksorttags[first]=qksorttags[last];
    while(first<last&&qksortfdc[first]<=key)
    ++first;
    qksortfdc[last]=qksortfdc[first];
    qksorttags[last]=qksorttags[first];
    }
    qksortfdc[first]=key;
    qksorttags[first]=keytag;
    qksort(qksorttags,qksortfdc,low,first-1);
    qksort(qksorttags,qksortfdc,last+1,high);
  }
  return 0;
}