#include "mysqlplus.h"

#include <stdio.h>
#include <stdlib.h>

#include "errorcode.h"

namespace demo{

Mysqlplus::Mysqlplus(){
}

Mysqlplus::~Mysqlplus(){}

int32_t Mysqlplus::init(const char* ip,const char* username,const char* password,const char* db){
    con = mysql_init(NULL);
    if(con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //exit(EXIT_FAILURE);
        return Failed;
    }
    
    if(!mysql_real_connect(con, ip, username, password, db, 0, NULL,0))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        //exit(EXIT_FAILURE);
        return Failed;
    }
    return Success;
}

int32_t Mysqlplus::query(const char* queryer){
    if(mysql_query(con, queryer)){
        fprintf(stderr, "%s\n", mysql_error(con));
        //mysql_close(con);
        //exit(EXIT_FAILURE);
        return Failed;
    }
    return Success;
}

std::map<std::string,std::map<std::string,std::string>> Mysqlplus::parser(){
    std::map<std::string,std::map<std::string,std::string>> mp;
    MYSQL_RES *res=mysql_store_result(con);
    int32_t num_fields= mysql_num_fields(res);
    MYSQL_FIELD *field=mysql_fetch_fields(res);
    std::string index; 
    while (MYSQL_ROW row = mysql_fetch_row(res)) //获取整条数据内容
    {
        for(int32_t i=0;i<num_fields;++i)
        {
            if(i==0){ 
                mp.insert(std::make_pair(row[i],std::map<std::string,std::string>()));
                index = row[i];
            }
            else{
                mp[index].insert(std::make_pair(field[i].name,row[i]));
            }
            printf("[%s]=>[%s]\n",field[i].name,row[i]);
        }  
    }
    mysql_free_result(res);
    printf("mariadb is connect and run succesed!mp_size is %d\n",(int)mp.size());
    
    return mp;
    //mysql_close(con);
}
}