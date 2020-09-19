#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 11
#define GAV 66742800.0
#define MS 332608.0
#define RA 1.0
#define me 5980000.0
#define Re 6371000.0
#define AU 149597870000.0
#define rm 384400000
#define YEAR 31536000.0
#define step 1

struct planet
{
    char * name;
    double mass;
    double radius;
    double x,y,vx,vy,ax,ay;
};
typedef struct planet *Pplanet;

Pplanet createPlanet(char * name,double mass,double  radius,double x,double y,double vx,double vy)
{
    Pplanet p=(Pplanet)malloc(sizeof(struct planet));
    if(p!=NULL)
    {
        p->name=(char *)malloc(sizeof(char)*20);
        strcpy(p->name, name);
        p->mass=mass;
        p->radius=radius;
        p->x=x;
        p->y=y;
        p->vx=vx;
        p->vy=vy;
    }
    else
        printf("create planet failed");
    return p;
}

void clean_acceleration(Pplanet p)
{
    p->ax=0;
    p->ay=0;
}

void calculate_acceleration(Pplanet p,double x,double y)
{
    p->ax+=x;
    p->ay+=y;
}

void calculate_velocity(Pplanet p)
{
    p->vx+=p->ax*step;
    p->vy+=p->ay*step;
}

void calculate_coordination(Pplanet p)
{
    p->x+=p->vx*step+0.5*p->ax*step*step;
    p->y+=p->vy*step+0.5*p->ay*step*step;
}

int main()
{
    int n=10;
    int i,j;
    unsigned long int count=0;
    FILE *fp[10];
    fp[0]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/sun.xls", "w");
    fp[1]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/mercury.xls", "w");
    fp[2]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/venus.xls", "w");
    fp[3]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/earth.xls", "w");
    fp[4]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/mars.xls", "w");
    fp[5]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/jupiter.xls", "w");
    fp[6]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/saturn.xls", "w");
    fp[7]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/uranus.xls", "w");
    fp[8]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/neptune.xls", "w");
    fp[9]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v7.0)/moon.xls", "w");
    for (i=1; i<9; i++)
    {
        if (fp[i]==NULL)
            printf("Cannot find the file %d\n",i);
        else fprintf(fp[i], "%s\t%s\t%s\n","Time","X","Y");
    }
    
    double force[MAX][MAX][2];
    Pplanet solar_system[n];
    solar_system[0]=createPlanet("sun",MS*me,109.13*Re,0.0,0.0,0.0,0);
    solar_system[1]=createPlanet("mercury",0.0553*me,0.38*Re,0.3871*AU,0.0,0.0,52716.3);
    solar_system[2]=createPlanet("venus",0.815*me,0.95*Re,0.7233*AU,0.0,0.0,35242);
    solar_system[3]=createPlanet("earth",me,Re,AU,0.0,0.0,29789);
    solar_system[4]=createPlanet("mars",0.1074*me,0.53*Re,1.523*AU,0.0,0.0,25310);
    solar_system[5]=createPlanet("mercury2",0.0553*me,0.38*Re,(-1)*0.3871*AU,0.0,0.0,52716.3);
    solar_system[6]=createPlanet("venus2",0.815*me,0.95*Re,(-1)*0.7233*AU,0.0,0.0,35242);
    solar_system[7]=createPlanet("earth2",me,Re,(-1)*AU,0.0,0.0,29789);
    solar_system[8]=createPlanet("mars2",0.1074*me,0.53*Re,(-1)*1.523*AU,0.0,0.0,25310);
    double t=0;
    
    while(t<=10*YEAR)
    {
        for(i=0;i<n;i++)
            for(j=i+1;j<n;j++)
            {
                double x1=solar_system[i]->x;
                double y1=solar_system[i]->y;
                double x2=solar_system[j]->x;
                double y2=solar_system[j]->y;
               
                double m1=solar_system[i]->mass;
                double m2=solar_system[j]->mass;
               
                double r1=solar_system[i]->radius;
                double r2=solar_system[j]->radius;
                
                double v1x=solar_system[i]->vx;
                double v1y=solar_system[i]->vy;
                double v2x=solar_system[j]->vx;
                double v2y=solar_system[j]->vy;
                
                double squareroot=sqrt(((v1x-v2x)*(v1x-v2x)+(v1y-v2y)*(v1y-v2y))/((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
                
                double f=GAV*m1*m2/(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
                force[i][j][0]=f*(x2-x1);
                force[i][j][1]=f*(y2-y1);
                force[j][i][0]=f*(x1-x2);
                force[j][i][1]=f*(y1-y2);
                
                if (r1+r2>=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)))
                {
                    solar_system[i]->vx=v1x+2*m2*(x1-x2)*squareroot/(m1+m2);
                    solar_system[i]->vy=v1y+2*m2*(y1-y2)*squareroot/(m1+m2);
                    solar_system[j]->vx=v2x+2*m1*(x2-x1)*squareroot/(m1+m2);
                    solar_system[j]->vy=v2y+2*m1*(y2-y1)*squareroot/(m1+m2);
                }
                
            }
        for (i=1;i<n;i++)
        {
            clean_acceleration(solar_system[i]);
            for (j=0;j<n;j++)
                if(i!=j)
                    calculate_acceleration(solar_system[i],force[i][j][0]/solar_system[i]->mass,force[i][j][1]/solar_system[i]->mass);
            
            calculate_coordination(solar_system[i]);
            calculate_velocity(solar_system[i]);
        }
        
        for (i=1; i<10; i++)
        {
            if (count%864000==0)
                fprintf(fp[i], "%lu\t%e\t%e\n",count/864000,solar_system[i]->x,solar_system[i]->y);
        }
        
        t+=step;
        count++;
    };
}