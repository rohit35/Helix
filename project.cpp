#include<stdio.h>
#include<windows.h>
#include<pthread.h>
#include<conio.h>
#include<string.h>
#include<graphics.h>
#include<time.h>
#define space "\t \t \t"
#define ON 1
#define OFF 0
void doCursor(int);
void newLine();
void newLine()
{
    moveto(0,gety()+textheight("A"));
}
void getGrString(char *inputString)
{
    int stringIndex=0;
    int xVal[255];
    char inputChar, outString[2];
    int oldColor;
    outString[1]=0;
    xVal[0]=getx();
    do
    {
        doCursor(ON);
        inputChar=getch();
        doCursor(OFF);
        if (inputChar==0) getch();
        else
        {
            if (inputChar==8)
            {
                oldColor=getcolor();
                --stringIndex;
                if (stringIndex<0) stringIndex=0;
                moveto(xVal[stringIndex],gety());
                setcolor(getbkcolor());
                outString[0]=inputString[stringIndex];
                outtext(outString);
                moveto(xVal[stringIndex],gety());
                setcolor(oldColor);
            }
            else
            {
                inputString[stringIndex]=inputChar;
                outString[0]=inputChar;
                outtext(outString);
                ++stringIndex;
                xVal[stringIndex]=getx();
            }
        }
    }
    while(inputChar!=13 && inputChar!=10);
    inputString[stringIndex]=0;
}
void doCursor(int on)
{
    int curX,oldColor;
    char uBarStr[2] = { '_',0 };
    if (!on)
    {
        oldColor=getcolor();
        setcolor(getbkcolor());
    }
    curX=getx();
    outtext(uBarStr);
    moveto(curX,gety());
    if (!on) setcolor(oldColor);
}
static int up_price=0;
static int ind_pos=0;
typedef struct cmp_data
{
    char name[100];
    char start_price[100];
    char volume[100];
    char market[100];
} cmp_data;
typedef struct account_details
{
    char name[100];
    int investement;
    int available_cash;
    int stock_value;
    int total_transaction;
} account_details;
typedef struct record
{
    char cmp_name[100];
    int no_of_share;//buyed
    char volume[100];//no of shares total present.
    int buy_price;
    char st_price[100];
    int selling_price;
    int percentage;//profit or loss gain
    char buy_time[40];
    char sell_time[40];
    char market[100];
    int pos;
    int active;
    char order_shares[100];
    int sell_cash;//recieved from shares
    int buy_cash;//given for shares;
    struct record*next;//next pointer to create new record;
} record;
typedef struct user_details
{
    char username[100];
    void *portfolio;
    void *watch_list;
    account_details *account;
    void *trans_his;
} user_details;
typedef struct database{
user_details* data;
char name[100];
struct database*next;
}database;
void getGrString(char *);
int array[20]= {0};
void graph(int value);
void fun(int exp,int act);
int  calculating_stock_value(user_details*file);
void check_null();
void copy(char *des,char *source);
char * printf_date(char*source);
void insert_record_1(user_details*file,record *r1);
void remove_record(user_details*file,int cur_pos);
void insert_record(user_details*file,record *r1,record *r2);
void sell_section(user_details*file,int cur_pos);
void status();
char *calculating_time();
int check_quantity_available(char *requested,char *available);
void updating_funds(user_details*file,int cmp_num,int shares);
int check_funds(char *quantity,int number,long long int cash);
int check_quantity(char *quantity);
void buy_section(int cmp_num,user_details* file);
void  updating_point();
void  signup_page();
void storing_data(user_details*cur_file);
char *percent_int_char(int src);
int* whole_data(int st_price,int page,int *length);
void fun_graph(int *data,int len,int st_price);
void graph_coordinates(int *prev_data,int len,int st_price);
void view_stock(cmp_data*Data,user_details*file,int pos);
void graph_section(char *name,char* shares,char* st_price,int page,int current);
void *myThreadFun(void *id);
char *int_char(int src);
void login_page();
void adding_to_watch_list(cmp_data*Data,int pos,user_details*file);
void trans_sec(user_details*file);
int char_to_int(char*num);
void user_choice(int choice,user_details *file);
void userpage(char *username);
int Main_page();
void insert_into_record(cmp_data*collect,char *Data,int pos);
void arrow_fun(int exp,int act);
void printf_sec(char *data);
void printf_name(char *name);
int  calcu_eps(char* capital,char* shares);
int calcu_pe(char*price,int eps);
int calcu_Div(char* price);
COORD coord= {0,0};
void *storage;
user_details* create_user_info(char *username)
{
    user_details *rcd=(user_details*)malloc(1*sizeof(user_details));
    strcpy(rcd->username,username);
    account_details *poin=(account_details*)malloc(1*sizeof(account_details));
    strcpy(poin->name,username);
    poin->investement=500000;
    poin->available_cash=500000;
    poin->stock_value=0;
    poin->total_transaction=0;
    rcd->account=poin;
    rcd->portfolio=NULL;
    rcd->trans_his=NULL;
    rcd->watch_list=NULL;
    return rcd;
}
user_details* checking_user_details(char *username)
{    if(storage==NULL)
    {
        storage=malloc(sizeof(database));
        storage=(database*)malloc(sizeof(database));
        database*cur=(database*)storage;
        cur->next=NULL;
        strcpy(cur->name,username);
        cur->data=create_user_info(username);
        return cur->data;
    }
    database*temp=(database*)storage;
    while(temp!=NULL && (strcmp(temp->name,username)!=0))
    {
        temp=temp->next;
    }
    if(temp!=NULL)
        return temp->data;
    else
    {
        temp=(database*)storage;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        database*cur_1=(database*)malloc(sizeof(database));
        cur_1->next=NULL;
        strcpy(cur_1->name,username);
        cur_1->data=create_user_info(username);
        temp->next=cur_1;
        return cur_1->data;
    }
}
void *myThreadFun(void *id)
{
    FILE * fp_3=fopen("Prices_list.txt","a+");
    char *str;
    time_t t;
    int random_1,idx,count=20;
    random_1  =idx=0;
    while(count--)
    {
        srand((unsigned) time(&t));
        random_1=rand()%10;
        Sleep(1000);
        if(random_1 == count)
        {
            if(random_1%2==0)
                str=int_char(random_1*2);
            else
                str=int_char(random_1*2*-1);
        }
        else
        {
            if(random_1%2!=0)
                str=int_char(random_1);
            else
                str=int_char(random_1*-1);
        }
        while(str[idx]!='\0')
        {
            fputc(str[idx],fp_3);
            idx++;
        }
        if(idx==0)
        {
            fputc('0',fp_3);
        }
        fputc('#',fp_3);
        idx=0;
    }
    fputc('$',fp_3);
    up_price++;
    fclose(fp_3);
    updating_point();
}
int main()
{
    int gdriver = DETECT, gmode, errorcode;
    initgraph(&gdriver, &gmode, "");
    errorcode = graphresult();
    if (errorcode != grOk)  /* an error occurred */
    {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to halt:");
        getch();
        return(1);
    }
    setbkcolor(BLACK);
    setcolor(YELLOW);
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    outtextxy(254,75,"WELCOME");
    newLine();
    outtextxy(310,130,"TO");
    newLine();
    outtextxy(225,175,"   HELIX");
    newLine();
    setcolor(GREEN);
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    outtextxy(240,300,"Press any Key\n");
    getch();
    pthread_t t;
    pthread_create(&t, NULL, myThreadFun, (void*)1);
    Main_page();
    return 0;
}
void userpage(char *username)
{
    user_details *file=checking_user_details(username);
    int select=1,loop=1;
user_sec:
    while(loop)
    {
        updating_point();
        if(select>7 ||select<1)
        {
            select =1;
        }
        cleardevice();
        setcolor(BLUE);
        outtext("        MY TRADE INFO  ");
        setcolor(YELLOW);
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        newLine();
        if(select ==1)
        {
            newLine();
            outtext("-> Portfolio");
        }
        else
        {
            newLine();
            outtext("   Portfolio");
        }
        if(select == 2)
        {
            newLine();
            outtext("-> Watchlist");
        }
        else
        {
            newLine();
            outtext("   Watchlist");
        }
        if(select==3)
        {
            newLine();
            outtext("-> Stock list\n");
        }
        else
        {
            newLine();
            outtext("   Stock list");
        }
        if(select==4)
        {
            newLine();
            outtext("-> Account");
        }
        else
        {
            newLine();
            outtext("   Account");
        }
        if(select ==5)
        {
            newLine();
            outtext("-> Transaction histroy");
        }
        else
        {
            newLine();
            outtext("   Transaction histroy");
        }
        if(select==6)
        {
            newLine();
            outtext("-> Help Section");
        }
        else
        {
            newLine();
            outtext("   Help Section");
        }
        if(select==7)
        {
            newLine();
            outtext("-> EXIT");
        }
        else
        {
            newLine();
            outtext("   EXIT");
        }
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(BLUE);
        outtextxy(150,300,"Use the Arrow key's to move and select");
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        setcolor(YELLOW);
        switch(getch())
        {
        case 'H':
            select--;
            break;
        case 'P':
            select++;
            break;
        case 13:
            if(select!=7)
            {
                user_choice(select,file);
            }
            else
            {
                storing_data(file);
                loop=0;
            }
            break;
        }
    }
}
void storing_data(user_details*file)
{
    database*cur=(database*)storage;
    while(strcmp(cur->name,file->username)!=0)
    {
        cur=cur->next;
    }
    cur->data=NULL;
    cur->data=file;
}
void Help_sec(char *Name)
{
    cleardevice();
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    setcolor(YELLOW);
    outtextxy(30,05,"New to Stock Guide??");
    outtextxy(250,05," Have a quick read then!! ");
    setcolor(BLUE);
    outtextxy(20,25,"->Stock Guide is an virtual Trading app");
    outtextxy(20,45,"->Which is based on Equity share's\n");
    setcolor(GREEN);
    outtextxy(20,65,"     <<<<Company Details>>>>>\n");
    setcolor(BLUE);
    outtextxy(5,85,">Company Name = 'XYZ'.\n");
    outtextxy(5,105,">Volume = No of shares available.");
    outtextxy(5,125,">Starting price = Starting price of each share.");
    outtextxy(5,145,">Market Capital = Total turn over capital of the company.");
    setcolor(GREEN);
    outtextxy(5,165,"   <<<<<Fundamental Details>>>>>>");
    setcolor(BLUE);
    outtextxy(5,185,">EPS = Earning per share.");
    outtextxy(5,205,">It is used to determine company stock Condition.");
    outtextxy(5,225,">Its supply and Demand in the market.");
    outtextxy(5,245,">if eps <= 26 then its is consider at under value share.");
    outtextxy(5,265,">if eps >26 then it is consider at over value share.");
    outtextxy(5,285,">PE ratio = Price Earnings.");
    outtextxy(5,305,">It is used to determine company financial condition.");
    outtextxy(5,325,">If its below 14 it is consider as good.");
    outtextxy(5,345,">Dividend = Payed by the company.");
    outtextxy(5,365,">To those who hold there company share for long term.");
    outtextxy(5,385,">Min range =1 year to max=unlimited as long the company exists..");
    setcolor(GREEN);
    outtextxy(20,405,"Graph section.");
    setcolor(BLUE);
    outtextxy(5,425,">It is used to show the company Finanial condition.");
    outtextxy(5,445,">Its previous performance ,also used to calculate ");
    outtextxy(5,465," the technical analysis.Press any Key");
    getch();
}
void portfolio(user_details *file)
{
    cleardevice();
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    setcolor(YELLOW);
    outtext("                 PORTFOLIO                 ");
    newLine();
    newLine();
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    if(file->portfolio==NULL)
    {
        outtext(" COMPANY_NAME  || SHARES  ||  CUR_PRICE || BUY_PRICE   " );
        newLine();
        newLine();
        outtext("    NULL       ||    NULL  ||    NULL   ||  NULL  ");
        newLine();
    }
    else
    {
        char x='0';
        int loop=1,arrow=0,total=1,times=0,y1=95;
        while(loop)
        {
            cleardevice();
             settextstyle(BOLD_FONT,HORIZ_DIR,0);
            setcolor(YELLOW);
            outtext("             Portfolio          ");
            newLine();
            newLine();
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            outtext(" COMPANY_NAME   || Shares  ||  Cur_price ||  Buy_priced  " );
            newLine();
            newLine();
            record *cur=(record*)file->portfolio;
            while(cur!=NULL)
            {
                arrow_fun(times,arrow);
                outtextxy(60,y1,cur->cmp_name);
                outtextxy(210,y1,int_char(cur->no_of_share));
                outtextxy(340,y1,int_char(array[cur->pos]));
                outtextxy(460,y1,int_char(cur->buy_price));
                newLine();
                y1+=20;
                cur=cur->next;
                times++;
            }
            total=times;
            times=0;
            newLine();newLine();
            arrow_fun(total,arrow);
            outtext("  Go back");
            newLine();
            outtext("  Use the arrow key's");
            newLine();
            outtext("  Press Enter for selling");
            x=getch();
            y1=95;
            switch(x)
            {
            case 'H':
                arrow--;
                break;
            case 'P':
                arrow++;
                break;
            case 13:
                if(arrow==total)
                    loop=0;
                else
                {
                    cur=(record*)file->portfolio;
                    while(cur!=NULL && arrow!=0)
                    {
                        cur=cur->next;
                        arrow--;
                    }
                    sell_section(file,cur->pos);
                    loop=0;
                }
                break;
            }
            if(arrow<0 ||arrow>total)
            {
                arrow=0;
            }
        }
    }
    outtextxy(250,420,"Press any key");
    getch();
}
void sell_section(user_details*file,int cur_pos)
{
    cleardevice();
    record*set=(record*)file->portfolio;
    while(set->pos!=cur_pos)
    {
        set=set->next;
    }
    char quantity[100];
    int idx=0,index=0;
set_up:
    if(idx==0)
    {
        record *r1=(record*)malloc(sizeof(record));
        record *r2=(record*)malloc(sizeof(record));
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        outtext("             Sell option          ");
        newLine();
        newLine();
        newLine();
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        outtextxy(100,100,"COMPANY NAME   :");
        outtextxy(280,100,set->cmp_name);
        newLine();
        newLine();
        outtextxy(100,130,"Shares        :");
        outtextxy(290,130,int_char(set->no_of_share));
        newLine();
        newLine();
        outtextxy(100,160,"Cur_price     :");
        outtextxy(280,160,int_char(array[set->pos]));
        newLine();
        char string[100];
        newLine();newLine();
        outtext("Enter the sell quantity :");
        getGrString(string);
        copy(quantity,string);
        if(check_quantity(quantity)==0)
        {
            idx++;
            goto set_up;
        }
        if(check_quantity_available(quantity,int_char(set->no_of_share))==0)
        {
            idx++;
            goto set_up;
        }
        if(set->active==1)
        {
            memcpy((void*)r1,(void*)set,sizeof(record));
            r1->next=NULL;
            r1->sell_cash=0;
            set->active=0;
            memcpy((void*)r2,(void*)set,sizeof(record));
            r2->next=NULL;
            r2->selling_price=array[r2->pos];
            r2->percentage=(r2->selling_price-r2->buy_price)*r2->no_of_share;
            set->percentage=r2->percentage;
            strcpy(r2->sell_time,calculating_time());
            r2->sell_cash=char_to_int(quantity)*array[r2->pos];
            r2->no_of_share-=char_to_int(quantity);
            strcpy(r2->order_shares,quantity);
            insert_record(file,r1,r2);
        }
        else
        {
            memcpy((void*)r1,(void*)set,sizeof(record));
            r1->next=NULL;
            r1->selling_price=array[r1->pos];
            r1->percentage=(r1->selling_price-r1->buy_price)*r1->no_of_share;
            strcpy(r1->sell_time,calculating_time());
            r1->sell_cash=char_to_int(quantity)*array[r1->pos];
            r1->no_of_share-=char_to_int(quantity);
            strcpy(r1->order_shares,quantity);
            set->percentage=r1->percentage;
            r2->sell_cash=0;
            insert_record_1(file,r1);
        }
        if(set->no_of_share==char_to_int(quantity))
        {
            remove_record(file,set->pos);
        }
        else
        {
            set->no_of_share-=char_to_int(quantity);
        }
        account_details *acc=file->account;
        acc->available_cash=acc->available_cash+r1->sell_cash+r2->sell_cash;
        acc->total_transaction++;
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(GREEN);
        outtextxy(250,400,"Your Selling order is succesfull");
        outtextxy(10,280,"Your account balance is:");
        outtextxy(290,280,int_char(acc->available_cash));
        outtextxy(10,320,"profit or loss gained is:");
        outtextxy(290,320,int_char(set->percentage));
        FILE *fp;
        fp=fopen("COMPANY_DATA.txt","r");
        char data[1000],x,org_quantity[100];
        while((x = fgetc(fp)) != EOF)
        {
            data[index++] = x;
        }
        data[index]='\0';
        fclose(fp);
        index=0;
        int pos=0,start_pos=0,last_pos=0;
        while(pos<set->pos && data[index]!='\0')
        {
            if(data[index]=='$')
            {
                pos++;
            }
            index++;
        }
        pos=0;
        while(pos<=1)
        {
            if(data[index]=='#')
            {
                pos++;
            }
            index++;
        }
        pos=0;
        start_pos=index;
        while(data[index]!='$')
        {
            org_quantity[pos]=data[index++];
            pos++;
        }
        last_pos=index;
        fp=fopen("COMPANY_DATA.txt","w");
        index=0;
        while(index<start_pos)
        {
            x=data[index++];
            fputc(x,fp);
        }
        index=0;
        char *s=int_char(char_to_int(org_quantity)+char_to_int(quantity));
        while(s[index]!='\0')
        {
            x=s[index++];
            fputc(x,fp);
        }
        index=last_pos;
        while(data[index]!='\0')
        {
            x=data[index++];
            fputc(x,fp);
        }
        fclose(fp);
    }
    else
    {
        outtextxy(330,420,"press any key");
        getch();
    }
}
void insert_record_1(user_details*file,record*r1)
{
    record*cur=(record*)file->trans_his;
    while(cur->next!=NULL)
    {
        cur=cur->next;
    }
    cur->next=r1;
}
void remove_record(user_details*file,int cur_pos)
{
    record *current=(record*)file->portfolio;
    record*cur_2=current;
    if(current->pos==cur_pos)
    {
        file->portfolio=current->next;
    }
    else
    {
        while(current->pos!=cur_pos)
        {
            current=current->next;
        }
        while(cur_2->next!=current)
        {
            cur_2->next=current->next;
        }
    }
}
void insert_record(user_details*file,record *r1,record *r2)
{
    if(file->trans_his==NULL)
    {
        file->trans_his=r1;
        r1->next=r2;
    }
    else
    {
        record *cur=(record*)file->trans_his;
        while(cur->next!=NULL)
        {
            cur=cur->next;
        }
        cur->next=r1;
        cur=cur->next;
        cur->next=r2;
    }
}
void watch_list(user_details *file)
{
    cleardevice();
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    setcolor(YELLOW);
    outtext("             Watch List          ");
    newLine();
    newLine();
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    if(file->watch_list==NULL)
    {
        outtext("COMPANY NAME||START_PRICE || VOLUME || CUR_PRICE ||MARKET CAPITAL");
        newLine();
        newLine();
        outtext("  NULL     ||  NULL       ||  NULL  ||  NULL     ||  NULL     ");
        newLine();
    }
    else
    {
        char x='0';
        record*cur;
        int loop=1,arrow=0,total=0,time=0,y1=95;
        while(loop)
        {
            cleardevice();
             settextstyle(BOLD_FONT,HORIZ_DIR,0);
            setcolor(YELLOW);
            outtext("             Watch List          ");
            newLine();
            newLine();
            cur=(record*)file->watch_list;
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            outtext("COMPANY NAME ||START_PRICE || VOLUME || CUR_PRICE ||MARKET CAPITAL");
            newLine();newLine();
            while(cur!=NULL)
            {
                arrow_fun(time,arrow);
                outtextxy(60,y1,cur->cmp_name);
                outtextxy(200,y1,cur->st_price);
                outtextxy(320,y1,int_char(cur->no_of_share));
                outtextxy(430,y1,int_char(array[cur->pos]));
                outtextxy(550,y1,cur->market);
                newLine();
                y1+=20;
                cur=cur->next;
                time++;
            }
            total=time;
            time=0;
            newLine();newLine();
            arrow_fun(total,arrow);
                 outtext("  Go back");
            newLine();
            outtext("  Use the arrow key's");
            newLine();
            outtext("  Press Enter for Entering View Stock");
            x=getch();
            y1=95;
            switch(x)
            {
            case 'H':
                arrow--;
                break;
            case 'P':
                arrow++;
                break;
            case 13:
                if(arrow==total)
                {
                    loop=0;
                }
                else
                {
                    cmp_data*Data=(cmp_data*)malloc(sizeof(cmp_data));
                    int var=arrow;
                    cur=(record*)file->watch_list;
                    while(var--)
                    {
                        cur=cur->next;
                    }
                    strcpy(Data->market,cur->market);
                    strcpy(Data->name,cur->cmp_name);
                    strcpy(Data->start_price,cur->st_price);
                    strcpy(Data->volume,int_char(cur->no_of_share));
                    view_stock(Data,file,cur->pos);
                }
                break;
            }
            if(arrow<0 || arrow>total)
            {
                arrow=total;
            }
        }
    }
     outtextxy(250,420,"Press any key");
    getch();
}
void inserting_the_data(cmp_data *collect,char*Data,int prev,int cur)
{
    int idx=0,position=0;
    if(cur==0 && prev!=cur)
    {
        insert_into_record(collect,Data,cur+1);
    }
    else if(cur==1 && prev!=cur)
    {
        position=cur*5;
        while(position--)
        {
            while(Data[idx]!='$')
            {
                idx++;
            }
            idx+=2;
        }
        insert_into_record(collect,Data,idx);
    }
    else if(cur==2 && prev!=cur)
    {
        position=10;
        while(position--)
        {
            while(Data[idx]!='$')
            {
                idx++;
            }
            idx+=2;
        }
        insert_into_record(collect,Data,idx);
    }
    else if(cur==3 && prev!=cur)
    {
        position=15;
        while(position--)
        {
            while(Data[idx]!='$')
            {
                idx++;
            }
            idx+=2;
        }
        insert_into_record(collect,Data,idx);
    }
}
void insert_into_record(cmp_data *collect,char *Data,int pos)
{
    int count=5,idx=0,range=0;
    char temp[100]= {0};
    while(range<count)
    {
        idx=0;
        while(Data[pos]!='#')
        {
            temp[idx++]=Data[pos];
            pos++;
        }
        temp[idx]='\0';
        pos++;
        strcpy(collect[range].name,temp);
        idx=0;
        temp[100]= {0};
        while(Data[pos]!='#')
        {
            temp[idx]=Data[pos++];
            idx++;
        }
        temp[idx]='\0';
        strcpy(collect[range].start_price,temp);
        idx=0;
        temp[100]= {0};
        pos++;
        while(Data[pos]!='$')
        {
            temp[idx]=Data[pos++];
            idx++;
        }
        temp[idx]='\0';
        pos+=2;
        strcpy(collect[range].volume,temp);
        temp[100]= {0};
        char *str=int_char((char_to_int(collect[range].start_price))*(char_to_int(collect[range].volume))*2);
        strcpy(collect[range].market,str);
        range++;
    }
}
void stock_list(user_details *file)
{
    cleardevice();
    FILE*fp=fopen("COMPANY_DATA.txt","r+");
    char x,cmp_details[1000];
    cmp_data *page_list =(cmp_data*)malloc(sizeof(cmp_data)*5);
    int idx=0,arrow=0;
    while((x = fgetc(fp)) != EOF)
    {
        cmp_details[idx++] = x;
    }
    cmp_details[idx] = '\0';
    int prev_page=4,cur_page=0,loop=1;
    while(loop)
    {
        cleardevice();
        inserting_the_data(page_list,cmp_details,prev_page,cur_page);
        prev_page=cur_page;
        setcolor(BLUE);
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        outtext("             STOCK LIST        ");
        newLine();
        setcolor(YELLOW);
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        outtext("COMPANIES || INITIAL_PRICE || VOLUME|| MARKET ||CUR_PRICE");
        arrow_fun(0,arrow);
        outtextxy(30,50,page_list[0].name);
        outtextxy(200,50,page_list[0].start_price);
        outtextxy(300,50,page_list[0].volume);
        outtextxy(400,50,page_list[0].market);
        outtextxy(540,50,int_char(array[(5*cur_page)+0]));
        if(arrow==0) outtextxy(600,50,"<<");
        else outtext("");
        outtextxy(30,100,page_list[1].name);
        outtextxy(200,100,page_list[1].start_price);
        outtextxy(300,100,page_list[1].volume);
        outtextxy(400,100,page_list[1].market);
        outtextxy(540,100,int_char(array[(5*cur_page)+1]));
        if(arrow==1) outtextxy(600,100,"<<");
        else outtext("");
        outtextxy(30,150,page_list[2].name);
        outtextxy(200,150,page_list[2].start_price);
        outtextxy(300,150,page_list[2].volume);
        outtextxy(400,150,page_list[2].market);
        outtextxy(540,150,int_char(array[(5*cur_page)+2]));
        if(arrow==2) outtextxy(600,150,"<<");
        else outtext("");
        outtextxy(30,200,page_list[3].name);
        outtextxy(200,200,page_list[3].start_price);
        outtextxy(300,200,page_list[3].volume);
        outtextxy(400,200,page_list[3].market);
        outtextxy(540,200,int_char(array[(5*cur_page)+3]));
        if(arrow==3) outtextxy(600,200,"<<");
        else outtext("");
        outtextxy(30,250,page_list[4].name);
        outtextxy(200,250,page_list[4].start_price);
        outtextxy(300,250,page_list[4].volume);
        outtextxy(400,250,page_list[4].market);
        outtextxy(540,250,int_char(array[(5*cur_page)+4]));
        if(arrow==4) outtextxy(600,250,"<<");
        else("");
        setcolor(YELLOW);
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(RED);
        outtextxy(310,320,"Page:");
        outtextxy(370,320,int_char(cur_page+1));
        outtextxy(380,320,"/4");
        outtextxy(230,290,"GO BACK");
        if(arrow==5) outtextxy(430,290,"<<");
        else("");
        setcolor(BLUE);
        outtextxy(150,370,"Use arrow keys to turn pages!");
        outtextxy(250,390,"< (prev)");
        outtextxy(250,420,"> (next)");
        x=getch();
        switch(x)
        {
        case 72:
            arrow--;
            break;
        case 80:
            arrow++;
            break;
        case 75:
            cur_page--;
            break;
        case 77:
            cur_page++;
            break;
        case 13:
            if(arrow==5)
                loop=0;
            else
            {
                cmp_data* cur=(cmp_data*)malloc(sizeof(cmp_data)*1);
                strcpy(cur->name,page_list[arrow].name);
                strcpy(cur->market,page_list[arrow].market);
                strcpy(cur->start_price,page_list[arrow].start_price);
                strcpy(cur->volume,page_list[arrow].volume);
                view_stock(cur,file,(cur_page*5)+arrow);
            }
        }
        if(arrow>5 ||arrow<0)
        {
            arrow=0;
        }
        if(cur_page>3 ||cur_page<0)
        {
            cur_page=0;
            arrow =0;
        }
    }
}
int  calcu_eps(char* capital,char* shares)
{
    int out_stan_shares=char_to_int(shares);
    return char_to_int(capital)/out_stan_shares-14;
}
int calcu_pe(char*price,int eps)
{
    return eps/char_to_int(price);
}
int calcu_Div(char* price)
{
    int dividend_yield=7;
    return char_to_int(price)/dividend_yield;
}

void updating_point()
{
    char Data[1000],x,max[3];
    int i,pos=0,in=0,count=0,idx=0;
    FILE *fp1,*fp;
    if((fp1=fopen("COMPANY_DATA.txt","r"))==NULL)
    {
        fp1=fopen("COMPANY_DATA.txt","w");
        char data[]="|venkys#40#500000$|satin#30#2500000$|ultratech#45#2000000$|polaris#57#250000$|Aditya vision#60#700000$|Mrf#85#1000000$|page#55#350000$|Ashok leyland#54#100000$|Tata#50#750000$|jio#40#100000$|kakatiya Cement#43#750000$|Coasboard Ind#47#500000$|Electrotham ind#61#210000$|Eichers#77#1000000$|Rana Sugar#53#500000$|Alkai Metals#49#450000$|Amazon#61#8500000$|JustDial#69#7200000$|Adobe#77#900000$|infoys#55#6500000$";
        i=0;
        while(data[i]!='\0')
        {
            x=data[i];
            fputc(x,fp1);
            i++;
        }
        fclose(fp1);
    }
    if(ind_pos==0)
    {
        int new_arr[20]= {40,30,45,57,60,85,55,54,50,40,43,47,61,77,53,49,61,69,77,55};
        for(idx=0; idx<20; idx++)
        {
            array[idx]=new_arr[idx];
        }
    }
    if(ind_pos<up_price)
    {
        fp=fopen("Prices_list.txt","a+");
        idx= pos=in=count =0;
        x='k';
        while(count<ind_pos)
        {
            x=fgetc(fp);
            if(x=='$')
            {
                count++;
            }
        }
        count=0;
        if(ind_pos!=0)
        {
            x='k';
        }
        while(x!='$')
        {
            x=fgetc(fp);
            Data[idx]=x;
            idx++;
        }
        Data[idx]='\0';
        while(in<idx)
        {
            while(Data[in]!='#' && Data[in]!='$')
            {
                max[pos++]=Data[in];
                in++;
            }
            if(Data[in]=='$')
            {
                in++;
            }
            else
            {
                if(pos==0)
                {
                    max[pos++]='0';
                }
                max[pos]='\0';
                pos=0;
                array[count]+=char_to_int(max);
                count++;
            }
            in++;
        }
        ind_pos++;
        up_price=ind_pos;
        pthread_t tid_2;
        pthread_create(&tid_2, NULL, myThreadFun, (void*)1);
    }
    check_null();
}
void check_null()
{
    int idx;
    for(idx=0; idx<20; idx++)
    {
        if(array[idx]<0)
            array[idx]=0;
    }
}
void view_stock(cmp_data*Data,user_details *file,int pos)
{
    char x;
    int EPS,PE,Dividend,choice=0,loop=1;
    EPS=calcu_eps(Data->market,Data->volume);
    PE=calcu_pe(Data->start_price,EPS);
    Dividend=calcu_Div(Data->start_price);
    while(loop)
    {
        cleardevice();
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        setcolor(YELLOW);
        outtext("                VIEW STOCK      ");
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        newLine();
        newLine();
        newLine();
        outtext("                     Company Details           ");
        newLine();
        outtext("              ***Fundamentals Analysis***");
        newLine();
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(BLUE);
        outtextxy(100,100,"          Name: ");
        outtextxy(280,100,Data->name);
        outtextxy(100,130,"          EPS :");
        outtextxy(280,130,int_char(EPS));
        outtextxy(100,160,"  No.of.Shares: ");
        outtextxy(280,160,Data->volume);
        outtextxy(100,190,"            PE: ");
        outtextxy(280,190,int_char(PE));
        outtextxy(100,220,"Starting price: ");
        outtextxy(280,220,Data->start_price);
        outtextxy(100,250,"      Dividend: ");
        outtextxy(280,250,int_char(Dividend));
        outtextxy(100,280,"Market Capital: ");
        outtextxy(280,280,Data->market);
        outtextxy(100,310," Current Price: ");
        outtextxy(280,310,int_char(array[pos]));
        setcolor(RED);
        newLine();
        fun(0,choice);
        outtextxy(230,340,"> View current graph");
        fun(1,choice);
        outtextxy(230,370,"> Buy");
        fun(2,choice);
        outtextxy(230,410,">Add to / Remove from  watch list");
        fun(3,choice);
        outtextxy(230,450,">Go back");
        x=getch();
        switch(x)
        {
        case 'H':
            choice--;
            break;
        case 'P':
            choice++;
            break;
        case 13:
            if(choice==3)
                loop=0;
            else if(choice==0)
            {
                cleardevice();
                graph_section(Data->name,Data->volume,Data->start_price,pos/5,pos%5);
                cleardevice();
            }
            else if(choice==1)
            {
                buy_section(pos,file);
            }
            else if(choice==2)
            {
                adding_to_watch_list(Data,pos,file);
            }
        }
        if(choice<0 ||choice>3)
        {
            choice=3;
        }
    }
}
void adding_to_watch_list(cmp_data*Data,int pos,user_details*file)
{
    int temp=0;
    record *check=(record*)file->watch_list;
    while(check!=NULL)
    {
        if(strcmp(check->cmp_name,Data->name)==0)
        {
            temp=1;
            break;
        }
        check=check->next;
    }
    check=(record*)file->watch_list;
    if(temp==1)
    {
        if(strcmp(check->cmp_name,Data->name)==0)
        {
            file->watch_list=check->next;
        }
        else
        {
            while(strcmp(check->cmp_name,Data->name)!=0)
            {
                check=check->next;
            }
            record*prev=(record*)file->watch_list;
            while(prev->next!=check)
            {
                prev=prev->next;
            }
            if(check->next==NULL)
            {
                prev->next=NULL;
            }
            else
            {
                prev->next=check->next;
            }
        }
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(GREEN);
        outtextxy(240,430,"Removed from Watch list\n");
    }
    else
    {
        record*rd=(record*)malloc(sizeof(record));
        rd->pos=pos;
        strcpy(rd->cmp_name,Data->name);
        strcpy(rd->market,Data->market);
        rd->no_of_share=char_to_int(Data->volume);
        strcpy(rd->st_price,Data->start_price);
        if(file->watch_list==NULL)
        {
            // file->watch_list=malloc(sizeof(record));
            file->watch_list=rd;
        }
        else
        {
            record*cur=(record*)file->watch_list;
            while(cur->next!=NULL)
            {
                cur=cur->next;
            }
            cur->next=rd;
        }
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(GREEN);
        outtextxy(240,390,"ADDED");
    }
    getch();
}
void buy_section(int cmp_num,user_details*file)
{
    int flag=0;
    cleardevice();
set_up:
    if(flag==0)
    {
        FILE *fp;
        fp=fopen("COMPANY_DATA.txt","r");
        char data[1000],x;
        int idx=0,pos=0,cur_pos=0,last_pos=0;
        while((x = fgetc(fp)) != EOF)
        {
            data[idx++] = x;
        }
        data[idx]='\0';
        fclose(fp);
        idx=0;
        while(pos<cmp_num && data[idx]!='\0')
        {
            if(data[idx]=='$')
            {
                pos++;
            }
            idx++;
        }
        pos=0;
        cur_pos=idx;
        char temp_data[100],cmp_name[100],shares[100];
        idx++;
        while(data[idx]!='$')
        {
            temp_data[pos]=data[idx++];
            pos++;
        }
        last_pos=idx;
        temp_data[pos]='\0';
        idx=pos=0;
        while(temp_data[idx]!='#')
        {
            cmp_name[idx]=temp_data[idx];
            idx++;
        }
        cmp_name[idx++]='\0';
        char str_pric[100];
        while(temp_data[idx]!='#')
        {
            str_pric[pos++]=temp_data[idx++];
        }
        str_pric[pos]='\0';
        pos=0;
        idx++;
        while(temp_data[idx]!='\0')
        {
            shares[pos]=temp_data[idx++];
            pos++;
        }
        shares[pos]='\0';
        account_details *sec;
        char quantity[100];
        sec=file->account;
        cleardevice();
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        setcolor(YELLOW);
        outtext("            Buy Section           ");
        newLine();
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        newLine();
        newLine();
        outtextxy(50,50,"   Compaany Name :");
        outtextxy(280,50,cmp_name);
        newLine();
        newLine();
        outtextxy(50,80,"  Company Shares :");
        outtextxy(280,80,shares);
        newLine();
        newLine();
        outtextxy(50,110,"   Current Price :");
        outtextxy(280,110,int_char(array[cmp_num]));
        newLine();
        newLine();
        outtextxy(50,150," available Funds :");
        outtextxy(280,150,int_char(sec->available_cash));
        newLine();
        char string[100];
        // outtextxy(50,190,"     Buy Quantity:");outtext("                       ");
        outtext("         Buy Quantity:");
        getGrString(string);
        copy(quantity,string);
        if(check_quantity(quantity)==0)
        {
            flag++;
            goto set_up;
        }
        if(check_quantity_available(quantity,shares)==0)
        {
            flag++;
            goto set_up;
        }
        if(check_funds(quantity,cmp_num,sec->available_cash)==0)
        {
            flag++;
            goto set_up;
        }
        record *set=(record*)malloc(1*sizeof(record));
        set->buy_price=array[cmp_num];
        strcpy(set->cmp_name,cmp_name);
        set->no_of_share=char_to_int(quantity);
        strcpy(set->order_shares,quantity);
        strcpy(set->volume,shares);
        set->buy_cash=char_to_int(quantity)*array[cmp_num];
        set->next=NULL;
        set->active=1;
        set->pos=cmp_num;
        strcpy(set->buy_time,calculating_time());
        strcpy(set->st_price,str_pric);
        if(file->portfolio==NULL)
        {
            file->portfolio=set;
        }
        else
        {
            record *list=(record*)file->portfolio;
            while(list->next!=NULL)
            {
                list=list->next;
            }
            list->next=set;
        }
        updating_funds(file,cmp_num,char_to_int(quantity));
        flag=1;
        idx=0;
        outtextxy(50,220,"  Current funds :");
        outtextxy(280,220,int_char(sec->available_cash));
        fp=fopen("Company_data.txt","w");
        while(idx<cur_pos)
        {
            x=data[idx];
            fputc(x,fp);
            idx++;
        }
        idx=0;
        fputc('|',fp);
        while(cmp_name[idx]!='\0')
        {
            x=cmp_name[idx++];
            fputc(x,fp);
        }
        idx=0;
        fputc('#',fp);
        while(str_pric[idx]!='\0')
        {
            x=str_pric[idx++];
            fputc(x,fp);
        }
        idx=0;
        fputc('#',fp);
        char *new_quantity=int_char(char_to_int(shares)-char_to_int(quantity));
        while(new_quantity[idx]!='\0')
        {
            x=new_quantity[idx++];
            fputc(x,fp);
        }
        idx=last_pos;
        while(data[idx]!='\0')
        {
            x=data[idx++];
            fputc(x,fp);
        }
        fclose(fp);
        account_details*cur=file->account;
        cur->total_transaction++;
        setcolor(GREEN);
        outtextxy(250,420,"Your Order is Succesfully Placed");
        getch();
    }
    else
    {
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(BLUE);
        outtextxy(250,440,"Press any Key");
        getch();
    }
}
char *calculating_time()
{
    char *cur_time;
    int len=0;
    while(len!=25)
    {
        time_t t;
        time(&t);
        cur_time=ctime(&t);
        len=strlen(cur_time);
    }
    return cur_time;
}
int check_quantity_available(char *requested,char *available)
{
    if(char_to_int(requested)<=char_to_int(available))
        return 1;
    else
    {
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        setcolor(RED);
        outtextxy(250,390,"Requested quantity Not available");
        return 0;
    }
}
void updating_funds(user_details*file,int cmp_num,int shares)
{
    account_details*sec=file->account;
    long long int cur=sec->available_cash;
    cur-=(array[cmp_num]*shares);
    sec->available_cash=cur;
}
int check_funds(char *quantity,int number,long long int cash)
{
    long long int shares=char_to_int(quantity);
    if(shares*array[number]<cash)
        return 1;
    else
        return 0;
}
void copy(char *des,char *source)
{
    int idx;
    for(idx=0;source[idx]!=13 && source[idx]!='\0';idx++)
    {
        des[idx]=source[idx];
    }
    des[idx]='\0';
}
int check_quantity(char *quantity)
{
    int idx=0;
    while(quantity[idx]!='\0' )
    {
        if(quantity[idx]>47 && quantity[idx]<58)
        {
            idx++;
        }
        else
        {
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            setcolor(BLUE);
            outtextxy(250,420,"Enter only Numbers");
            return 0;
        }
    }
    return 1;
}
int* whole_data(int st_price,int page,int CURRENT,int *length)
{
    FILE *fp;
    char x='k',Data[10000],max_array[3];
    fp=fopen("Prices_list.txt","a+");
    int pos=0,index=0,new_arr[1000],price=0,var=(5*page)+CURRENT,value=0;
    while(value<ind_pos)
    {
        index=0;
        while(x!='$')
        {
            x=fgetc(fp);
            Data[index]=x;
            index++;
        }
        Data[index]='\0';
        int cur=0,low=0;
        while(cur!=var && low<index)
        {
            if(Data[low]=='#')
            {
                cur++;
            }
            low++;
        }
        cur=0;
        while(Data[low]!='#')
        {
            max_array[cur++]=Data[low];
            low++;
        }
        max_array[cur]='\0';
        int v;
        v=char_to_int(max_array);
        new_arr[value]=v;
        value++;
        pos++;
        x='k';
    }
    int *r=&value;
    *length=*r;
    return new_arr;
}
void graph_section(char *name,char* shares,char* st_price,int page,int CURRENT)
{
    int len,*data;
    data=whole_data(char_to_int(st_price),page,CURRENT,&len);
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
    setcolor(RED);
    outtextxy(275,0,"Graph Section");
    outtextxy(50,20,"Name  :");
    outtextxy(120,20,name);
    outtextxy(50,40,"Shares :");
    outtextxy(120,40,shares);
    outtextxy(450,40,"Cur_price :");
    outtextxy(545,40,int_char(array[(5*page)+CURRENT]));
    setcolor(GREEN);
    line(0,60,639,60);
    if(len==0)
    {
        outtextxy(10,430-char_to_int(st_price),st_price);
        line(10,430,10,430-char_to_int(st_price));
    }
    else
    {
        fun_graph(data,len,char_to_int(st_price));
    }
    line(0,430,639,430);
    setcolor(BLUE);
    outtextxy(150,440,"Press any Key");
    getch();
}
void fun_graph(int *data,int len,int st_price)
{
    graph_coordinates(data,len,st_price);
    int add=0;
    if(len<2)
    {
        add=2;
    }
    int dist =600/(len+add);
    int idx=0,range=len+1;
    char *str=int_char(st_price);
    int x1=10,x2=10,y1=430,y2=430-st_price;
    while(idx<range)
    {
        outtextxy(x2,y2,str);
        line(x1,y1,x2,y2);
        x1=x2;
        y1=y2;
        x2=x2+dist;
        y2=430-data[idx]-05;
        str=int_char(data[idx]);
        idx++;
    }
}
void graph_coordinates(int *prev_data,int len,int st_price)
{
    int idx,cur=st_price;
    int *data=(int*)malloc(len*sizeof(int));
    for(idx=0; idx<len; idx++)
    {
        data[idx]=prev_data[idx];
        cur+=data[idx];
        data[idx]=cur;
        if(data[idx]<0)
        {
            data[idx]=0;
            cur=0;
        }
    }
    cur=0;
    int pos=0,diff=0;
    for(idx=0; idx<len; idx++)
    {
        if(data[idx]>360)
        {
            cur=data[idx]-360+2;
            pos=idx;
            data[idx]-=cur;
            while(pos--)
            {
                diff=data[pos+1]-data[pos];
                if(diff>=cur)
                {
                    pos=0;
                }
                else
                {
                    data[pos]-=cur;
                }
            }
        }
    }
    for(idx=0; idx<len; idx++)
    {
        prev_data[idx]=data[idx];
    }
}
void printf_sec(char *data)
{
    int len=strlen(data);
    printf("%s",data);
    if(len<12)
    {
        while(len<12)
        {
            printf(" ");
            len++;
        }
    }
}
void printf_name(char *name)
{
    int len=strlen(name);
    printf("%s",name);
    if(len<25)
    {
        while(len<25)
        {
            printf(" ");
            len++;
        }
    }
}
void my_Account(user_details *file)
{
    cleardevice();
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    setcolor(YELLOW);
    outtext("             My Account ");
    account_details*cur=file->account;
    int stock_value=calculating_stock_value(file);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    setcolor(BLUE);
    outtextxy(50,50,"               Name:");
    outtextxy(280,50,file->username);
    outtextxy(50,80,"Starting Investment:");
    outtextxy(280,80,int_char(cur->investement));
    outtextxy(50,110,"           Balance:");
    outtextxy(280,110,int_char(cur->available_cash));
    outtextxy(50,140,"       Stock Value:");
    outtextxy(280,140,int_char(stock_value));
    outtextxy(50,170,"Total Transactions:");
    outtextxy(280,170,int_char(cur->total_transaction));
    outtextxy(50,200,"       Total Value:");
    outtextxy(280,200,int_char(stock_value+cur->available_cash));
    if((cur->available_cash+stock_value)>cur->investement)
    {
        setcolor(GREEN);
        outtextxy(150,240,"PROFIT :) ");
    }
    else
    {
        setcolor(RED);
        outtextxy(150,240,"LOSS :( ");
    }
    setcolor(RED);
    outtextxy(200,300,">>press enter to see the : PIE CHART");
    outtextxy(200,330,">>press any other key to :  GO BACK");
    if(getch()==13)
    {
        graph(stock_value);
    }
    getch();
}
void graph(int value)
{
    cleardevice();
    int angle=0;
    if(value<500000 && value!=0)
    {
        if(value<5000)
        {
            angle=3.6;
        }
        else
        {
            angle=3.6*value/5000;
        }
        int midx,midy;
        setcolor(BLUE);
        rectangle(0,40,639,450);
        settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
        setcolor(WHITE);
        outtextxy(275,10,"PIE_CHART");
        midx=getmaxx()/2;
        midy=getmaxy()/2;
        setfillstyle(4,YELLOW);
        pieslice(midx,midy,0,angle,100);
        setcolor(YELLOW);
        int check=value/5000;
        if(check==0)
        {
            check=1;
        }
        outtextxy(midx+100,midy-30,percent_int_char(check));
        outtextxy(midx+100,midy-05,"stock _value");
        setfillstyle(6,BLUE);
        pieslice(midx,midy,angle,360,100);
        setcolor(BLUE);
        outtextxy(midx-10,340,percent_int_char(100-check));
        outtextxy(midx+10,360,"CASH");
        outtextxy(240,400,"Press any key");
    }
    else
    {
        int midx,midy;
        setcolor(BLUE);
        rectangle(0,40,639,450);
        settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
        setcolor(WHITE);
        outtextxy(275,10,"PIE_CHART");
        midx=getmaxx()/2;
        midy=getmaxy()/2;
        setfillstyle(4,BLUE);
        pieslice(midx,midy,0,360,100);
        setcolor(BLUE);
        outtextxy(midx+100,midy-30,"100%");
        outtextxy(midx+100,midy-05,"cash");
        outtextxy(240,400,"Press any key");
    }
    getch();
}
char *percent_int_char(int src)
{
    char r,*string=(char*)malloc(100*sizeof(char));
    int n=src,j=0,temp=0;
    if(src<0)
    {
        n=n*-1;
        temp=1;
    }
    string[j++]='%';
    if(n==0)
    {
        string[j++]='0';
    }
    while(n!=0)
    {
        r=n%10;
        string[j++]=(r+48);
        n/=10;
    }
    if(temp==1)
    {
        string[j++]=45;
    }
    string[j]='\0';
    strrev(string);
    return string;
}
int  calculating_stock_value(user_details*file)
{
    int money=0;
    record*data=(record*)file->portfolio;
    while(data!=NULL)
    {
        money +=data->no_of_share*array[data->pos];
        data=data->next;
    }
    return money;
}
void user_choice(int choice,user_details* file)
{
    if(choice==1)
        portfolio(file);
    else if(choice==2)
        watch_list(file);
    else if(choice==3)
        stock_list(file);
    else if(choice==4)
        my_Account(file);
    else if(choice==5)
        trans_sec(file);
    else if(choice==6)
        Help_sec(file->username);
}
void arrow_fun(int exp,int act)
{
    if(exp==act)
        outtext(space"->");
    else
        outtext(space"  ");
}
int Main_page()
{
    cleardevice();
    FILE *fp;
    char choice,x;
    char users[100];
    char username[100];
    char password[100];
    int i=0;
main_sec:
    cleardevice();
    while(1)
    {
        setcolor(YELLOW);
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        outtext("              STOCKGUIDE:");
        newLine();
        outtext("     1)login");
        newLine();
        outtext("     2)signup");
        newLine();
        outtext("     3)exit");
        newLine();
        outtext("Enter your Choice:");
        choice=getch();
        if(choice== 49 || choice ==50 ||choice==51)
            break;
        else
        {
            setcolor(RED);
            outtextxy(225,175,"Enter valid choice!!");
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            outtextxy(240,300,"Press any Key\n");
            getch();
            cleardevice();
        }
    }
    if(choice==49)
    {
        char x,*tempstr;
        char accountsDB[100];
        cleardevice();
        newLine();
        outtext("                LOGIN: ");
        newLine();
        newLine();
        newLine();
        outtext("        Username:");
        getGrString(username);
        newLine();
        i=0;
        while(username[i] != '\0')
        {

            users[i] = username[i];
            i++;
        }
        users[i] = '\0';
        i=0;
        newLine();
        outtext("        Password:");
        int i=0;
        int xVal[255];
        char  outString[2];
        int oldColor;
        outString[1]=0;
        xVal[0]=getx();
        while ((x = getch()) != '\r')
        {
            if (x==8 || x==127)
            {
                oldColor=getcolor();
                --i;
                if (i<0) i=0;
                moveto(xVal[i],gety());
                setcolor(getbkcolor());
                outString[0]=password[i];
                outtext(outString);
                moveto(xVal[i],gety());
                setcolor(oldColor);
            }
            else
            {
                outtext("*");
                password[i++] = x;
                xVal[i]=getx();
            }
        }
        password[i] = '\0';
        i = 0;
        strcat(username, password);
        fp = fopen("PROJECT_DATA_FILE.txt","r");
        while((x = fgetc(fp)) != EOF)
        {
            accountsDB[i++] = x;
        }
        accountsDB[i] = '\0';
        fclose(fp);
        if(NULL == strstr(accountsDB, username))
        {
            setcolor(RED);
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            newLine();
            outtextxy(230,300,"*Username or password is wrong  ");
            getch();
            goto main_sec;
        }
        else
        {
            tempstr = strstr(accountsDB, username);
        }
        i=0;
        while(username[i]!='\0')
        {
            if(username[i]!=tempstr[i])
                break;
            i++;
        }
        if(i==strlen(username))
        {
            userpage(users);
            goto main_sec;
        }
        else
        {
            setcolor(RED);
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            newLine();
            newLine();
            outtextxy(230,300,"*Username or password is wrong!");
            getch();
            goto main_sec;
        }
    }
    if(choice ==50)
    {
        cleardevice();
        outtext("             SIGNUP!");
        newLine();
        newLine();
        outtext("             Username:");
        getGrString(username);
        i=0;
        while((x = username[i]) != '\0')
        {
            if(x == '|' || x == '#' || x == '$')
            {
                settextstyle(BOLD_FONT,HORIZ_DIR,1);
                setcolor(RED);
                outtext("  *Username must not contain special characters like |, #, $ etc.");
                getch();
                goto main_sec;
            }
            i++;
        }
        char accountsDB[100];
        fp = fopen("PROJECT_DATA_FILE.txt","r");
        i = 0;
        while((x = fgetc(fp)) != EOF)
        {
            accountsDB[i++] = x;
        }
        fclose(fp);
        char temp_pass[100];
        if(NULL == strstr(accountsDB, username))
        {
            newLine();
            newLine();
            outtext("          Password:");
            int i=0;
            int xVal[255];
            char  outString[2];
            int oldColor;
            outString[1]=0;
            xVal[0]=getx();
            while ((x = getch()) != '\r')
            {
                if (x==8 || x==127)
                {
                    oldColor=getcolor();
                    --i;
                    if (i<0) i=0;
                    moveto(xVal[i],gety());
                    setcolor(getbkcolor());
                    outString[0]=password[i];
                    outtext(outString);
                    moveto(xVal[i],gety());
                    setcolor(oldColor);
                }
                else
                {
                    outtext("*");
                    password[i++] = x;
                    xVal[i]=getx();
                }
            }
            password[i] = '\0';
            newLine();
            newLine();
            outtext("Re-Enter password:");
            i=0;
            outString[1]=0;
            xVal[0]=getx();
            while ((x = getch()) != '\r')
            {
                if (x==8 || x==127)
                {
                    oldColor=getcolor();
                    --i;
                    if (i<0) i=0;
                    moveto(xVal[i],gety());
                    setcolor(getbkcolor());
                    outString[0]=temp_pass[i];
                    outtext(outString);
                    moveto(xVal[i],gety());
                    setcolor(oldColor);
                }
                else
                {
                    outtext("*");
                    temp_pass[i++] = x;
                    xVal[i]=getx();
                }
            }
            temp_pass[i] = '\0';
            i = 0;
            if(strcmp(temp_pass,password)==0)
            {
                i = 0;
                fp = fopen("PROJECT_DATA_FILE.txt","a");
                fputc('|',fp);
                while((x = username[i]) != '\0')
                {
                    fputc(x,fp);
                    i++;
                }
                i = 0;
                while((x = password[i]) != '\0')
                {
                    fputc(x,fp);
                    i++;
                }
                fputc('$', fp);
                fputc('1', fp);
                fputc('#', fp);
                fputc('1', fp);
                fclose(fp);
                newLine();
                setcolor(GREEN);
                outtextxy(220,300,"Sign up successful!");
                getch();
                goto main_sec;
            }
            else
            {
                newLine();
                settextstyle(BOLD_FONT,HORIZ_DIR,1);
                setcolor(RED);
                outtext("*Passwords do not match, try again");
                getch();
                goto main_sec;
            }
        }
        else
        {
            newLine();
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            setcolor(RED);
            newLine();
            newLine();
            outtext("     *Username is already taken!");
            getch();
            goto main_sec;
        }
    }
    if(choice==51)
    {
        cleardevice();
        newLine();
        setcolor(BLUE);
        settextstyle(BOLD_FONT,HORIZ_DIR,0);
        outtextxy(300,100,"EXIT ");
        newLine();
        settextstyle(BOLD_FONT,HORIZ_DIR,1);
        outtextxy(100,200,"  ---Are You Sure to exit this page ?---");
        newLine();
        newLine();
        outtextxy(200,250,"*** Enter 'y or n' *** ");
        newLine();
        x=getch();
        if(x=='n'||x=='N')
            goto main_sec;
        else
        {
            cleardevice();
            setcolor(YELLOW);
            settextstyle(BOLD_FONT,HORIZ_DIR,0);
            outtextxy(100,200," Thank u for visiting!!");
            setcolor(RED);
            outtextxy(230,250,"HELIX");
            settextstyle(BOLD_FONT,HORIZ_DIR,1);
            setcolor(GREEN);
            outtextxy(250,330,"Press any key!");
            getch();
            newLine();
        }
    }
}
int char_to_int(char*num)
{
    int length=strlen(num);
    int var=1;
    int i,c=0,temp;
    for(i=0; i<length; i++)
    {
        if(i==0 && num[i]==45 )
        {
            var=-1;
        }
        else
        {
            temp=(num[i]-48);
            c=c*10+temp;
        }
    }
    c=c*var;
    return c;
}
char *int_char(int src)
{
    char r,*string=(char*)malloc(100*sizeof(char));
    int n=src,j=0,temp=0;
    if(src<0)
    {
        n=n*-1;
        temp=1;
    }
    if(n==0)
    {
        string[j++]='0';
    }
    while(n!=0)
    {
        r=n%10;
        string[j++]=(r+48);
        n/=10;
    }
    if(temp==1)
    {
        string[j++]=45;
    }
    string[j]='\0';
    strrev(string);
    return string;
}
void trans_sec(user_details* file)
{
    cleardevice();
    settextstyle(BOLD_FONT,HORIZ_DIR,0);
    setcolor(YELLOW);
    outtext("             Transaction Histroy            ");
    newLine();
    newLine();
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtext("COMPANY_NAME   ||  OPTION   ||   SHARE  || PRICE  ||  TIME   ||  PERCENTAGE ");
    newLine();
    newLine();
    record*cur=(record*)file->trans_his;
    if(file->trans_his==NULL)
    {
        outtext(" NULL      ||    NULL   ||    NULL   || NULL  ||   NULL   ||   NULL   ");
    }
    else
    {

        char x;
        int var=0,loop=1,total=0,choice=0;
        while(cur!=NULL)
        {
            cur=cur->next;
            total++;
        }
        while(loop)
        {
            cur=(record*)file->trans_his;
            cleardevice();
            var=choice;
            while(choice-- && cur!=NULL )
            {
                cur=cur->next;
            }
            outtext("                  ***Transaction Histroy***");newLine();newLine();
            outtextxy(40,50,"COMPANY_NAME: ");
            outtextxy(250,50,cur->cmp_name);
            if(cur->active==1)
            {
                outtextxy(40,70,"  OPTION    :");
                outtextxy(250,70,"BUY");
                outtextxy(40,90,"  SHARE     :");
                outtextxy(250,90,int_char(cur->no_of_share));
                outtextxy(40,110," PRICE      :");
                outtextxy(250,110,int_char(cur->buy_price));
                outtextxy(40,130,"  TIME      :");
                outtextxy(250,130,printf_date(cur->buy_time));
                outtextxy(40,150," PERCENTAGE :");
                outtextxy(250,150,"0");
            }
            else
            {
                outtextxy(40,70,"  OPTION    :");
                outtextxy(250,70,"SELL");
                outtextxy(40,90,"  SHARE     :");
                outtextxy(250,90,(cur->order_shares));
                outtextxy(40,110," PRICE      :");
                outtextxy(250,110,int_char(cur->selling_price));
                outtextxy(40,130,"  TIME      :");
                outtextxy(250,130,printf_date(cur->sell_time));
                outtextxy(40,150," PERCENTAGE :");
                outtextxy(250,150,int_char(cur->percentage));
            }
            outtextxy(50,200,"Press Enter for Next page..");
            outtextxy(50,220,"To Go Back Press Any other key..");
           outtextxy(360,240,"Page:");
           outtextxy(440,240,int_char(var+1));
            x=getch();
            switch(x)
            {
            case 13:
                choice=var;
                choice++;
                break;
            case 27:
                loop=0;
                break;
                default:loop=0;
                break;
            }
            if(choice==total)
            {
                loop=0;
            }
        }
    }
    outtextxy(280,420,"Press any key");
    getch();
}
char * printf_date(char*source)
{
    int idx=4,i=0;
    char des[10];
    while(idx<10)
    {
        des[i++]=source[idx];
        idx++;
    }
    des[i++]=' ';
    des[i]='\0';
    return des;
}
void fun(int exp,int act)
{
    if(exp==0)
    {
        if(exp==act)
            outtextxy(220,340,">>>");
        else
            outtextxy(220,340,"  ");
    }
    else if(exp==1)
    {
        if(exp==act)
            outtextxy(220,370,">>>");
        else
            outtextxy(220,370,"  ");
    }
    else if(exp==2)
    {
        if(exp==act)
            outtextxy(220,410,">>>");
        else
            outtextxy(220,410,"  ");
    }
    else if(exp==3)
    {
        if(exp==act)
            outtextxy(220,450,">>>");
        else
            outtextxy(220,450,"  ");
    }
}
