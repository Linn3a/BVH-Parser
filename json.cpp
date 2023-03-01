#include<iostream>
#include<fstream>
#include"bvh_parser.h"

using namespace std;
using std::ofstream;
//写文件
void write(joint* root,ofstream &w)
{
    //其实是前序遍历
    if(root==NULL) return;
    else{
        //
        int i,j;
        w<<'{'<<endl;
        w<<"\"type\":"<<'"'<<root->joint_type<<'"'<<','<<endl;
        w<<"\"name\":"<<'"'<<root->name<<'"'<<','<<endl;
        w<<"\"offset\":["<<root->offset_x<<','<<root->offset_y<<','<<root->offset_z<<"],"<<endl;
        
        w<<"\"channels\":[";
        if(!root->channels.size())
        {w<<"],"<<endl;}
        else{

        for(i=0;i<(root->channels.size()-1);i++)
        w<<'"'<<root->channels[i]<<'"'<<',';
        w<<'"'<<root->channels[i]<<'"'<<"],"<<endl;
        }

        w<<"\"motion\":["<<endl;
        if(!root->motion.size())
        {w<<"],"<<endl;}
        else{
        for(i=0;i<root->motion.size()-1;i++)
        {
            w<<'[';
            for(j=0;j<root->motion[i].size()-1;j++)
            w<<root->motion[i][j]<<',';
            w<<root->motion[i][j];
            w<<"],"<<endl;
        }
        w<<'[';
            for(j=0;j<root->motion[i].size()-1;j++)
            w<<root->motion[i][j]<<',';
            w<<root->motion[i][j];
            w<<']'<<endl;
        w<<"],"<<endl;
        }
        w<<"\"children\":["<<endl;
        if(root->joint_type!="End"){
        //写迭代器吧
        vector<joint*>::iterator p=root->children.begin();
        while(p!=root->children.end()-1)
        {
            write(*p,w);
            w<<"},"<<endl;
            p++;
        }
            write(*p,w);
            w<<"}"<<endl;
            w<<"]"<<endl;
    }
    else{

                    w<<']'<<endl;
        }}
}

void jsonify(joint root, META meta_data) {
    ofstream outfile;
    outfile.open("output.json");
    outfile<<'{'<<endl;
    outfile<<"\"frame\":"<<meta_data.frame<<','<<endl;
    outfile<<"\"frame_time\":"<<meta_data.frame_time<<','<<endl;
    outfile<<"\"joint\":"<<endl;
    write(&root,outfile);
    outfile<<'}'<<endl;
    outfile<<'}'<<endl;

    // output the root and meta_data


    outfile.close();
}
//C:\Users\cos\Desktop\DSProject\json.cpp