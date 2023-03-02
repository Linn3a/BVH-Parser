#include<fstream>
#include<stdlib.h>
#include<iostream>
#include"bvh_parser.h"
using namespace std;
// a naive bvh parser
using std::ifstream;
using std::ofstream;
// 递归 写一个create函数，读字符串-字符串匹配，create的返回值应该是joint指针
// 就读文件，读到 ‘{’调下一层 读到‘}’return 
string Clearspace(string s)
{
    if(s.empty()) return s;
    else {s=s.erase(0,s.find_first_not_of(' '));
        s=s.erase(0,s.find_first_not_of('\t'));
        if(s.find_first_of('\r')<s.length());
        s=s.substr(0,s.find_first_of('\r'));
        if(s.find_first_of('\n')<s.length());
        s=s.substr(0,s.find_first_of('\n'));
        }
        return s;
}

joint* create(ifstream &f,string type,string name){
    string line;
    joint* r=new joint;
    r->joint_type=type;
    r->name=name;
    while (1)
    {
        getline(f,line);
        // getline(f,nextline);
        //删空白符
        line=Clearspace(line);
    
        if(!(line.substr(0,1)).compare("}")) return r;
        if(!(line.substr(0,6)).compare("OFFSET")){
            //OFFSET 0 20.6881 -0.73152
            int loc_space1=line.find_first_of(' ');
            int loc_space2=line.find_first_of(' ',loc_space1+1);
            string temp=line.substr(loc_space1+1,loc_space2-loc_space1-1);
            r->offset_x=atof(temp.c_str());
            
            loc_space1=loc_space2;
            loc_space2=line.find_first_of(' ',loc_space2+1);
            temp=line.substr(loc_space1+1,loc_space2-loc_space1-1);
            r->offset_y=atof(temp.c_str());
            
            loc_space1=loc_space2;
            loc_space2=line.find_first_of(' ',loc_space2+1);
            temp=line.substr(loc_space1+1,loc_space2-loc_space1-1);
            r->offset_z=atof(temp.c_str());
        }
        
            //可以确定的是{前面一定是命名的 可能是end 可能是JOINT
           else if(!(line.substr(0,5)).compare("JOINT"))
            {
                string n=line.substr(6,line.length()-6);
                r->children.push_back(create(f,"JOINT",n));
            }
            else if(!(line.substr(0,3)).compare("End"))//end 
            {
                r->children.push_back(create(f,"End",name+"_end"));
            } 
            else if(!(line.substr(0,8)).compare("CHANNELS"))
            {int len=line[9]-'0';
            for(int i=0;i<len;i++)
            {
                int loc_space1,loc_space2;
                if(i==0) loc_space1=10;
                else loc_space1=loc_space2;
                loc_space2=line.find_first_of(' ',loc_space1+1);
                r->channels.push_back(line.substr(loc_space1+1,loc_space2-loc_space1-1));
            }
            }   
    }
    return r;
};
// 读文件 

//前序遍历一棵树
void readmotion(string &mline,joint* root)
{
    //cout<<root->name<<endl;
    if(!root->joint_type.compare("End")) return;
    if(root!=NULL)
    {
        int len=(root->channels).size();
        vector<double> v;
        for(int j=0;j<len;j++)
        {
            int space=mline.find_first_of(' ');
            v.push_back(atof((mline.substr(0,space)).c_str()));
            mline.erase(0,space+1);
        }
        root->motion.push_back(v);
        for(int i=0;i<root->children.size();i++)
           // {if(root->children[i]->joint_type!="End")
            readmotion(mline,root->children[i]);
        }
    }



int main(int argc, char** argv) {
    joint root;
    META meta_data;
    ifstream file(argv[1]); //这里实例化了一个输入流对象 并打开

    string line;
    
    getline(file,line);
    if((line.substr(0,9)).compare("HIERARCHY")) 
    {   cout<<"不合格的bvh文件"<<endl;
        return 0;}
    getline(file,line);
    int space1=line.find_first_of(' ');
    string name=line.substr(space1+1,line.length()-space1-2);
    joint* r=new joint;
    getline(file,line);
    if(!(line.substr(0,1)).compare("{")) r=create(file,"ROOT",name);
    
    getline(file,line);
    line=Clearspace(line);

    getline(file,line);
    space1=line.find_first_of(':');
    meta_data.frame=atoi(line.substr(space1+1,line.length()-space1-1).c_str());

    getline(file,line);
    space1=line.find_first_of(':');
    meta_data.frame_time=atof(line.substr(space1+1,line.length()-space1-1).c_str());
    //cout<<frame_time<<endl;
    for(int i=0;i<meta_data.frame;i++)
    { getline(file,line);
    readmotion(line,r);}
    
    
    root=*r;
    jsonify(root, meta_data);
    file.close();

    return 0;
}
