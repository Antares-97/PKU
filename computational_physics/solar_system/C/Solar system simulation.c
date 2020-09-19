#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 11
#define GAV 66742800.0
#define MS 332608.0                                                                                                                    
#define RA 1.0
#define me 5980000.0
#define AU 149597870000.0
#define rm 384400000
#define YEAR 31536000.0
#define step 0.1

struct planet
{
    char name[10];
    double mass;
    double x,y,vx,vy,ax,ay;
};
typedef struct planet *Pplanet;

Pplanet createPlanet(char name1[10],double mass1,double rx1,double ry1,double vx1,double vy1)
{
    Pplanet p=(Pplanet)malloc(sizeof(struct planet));
    if(p!=NULL)
    {int i=0;
        for(;i<10;i++) p->name[i]=name1[i];
        p->mass=mass1;
        p->x=rx1;
        p->y=ry1;
        p->vx=vx1;
        p->vy=vy1;
    }
    else
        printf("create planet failed");
    return p;
}

void clean_acceleration(Pplanet p)
{
    p->ax=0;p->ay=0;
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
    FILE *fp[11];
    fp[0]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/sun.xls", "w");
    fp[1]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/mercury.xls", "w");
    fp[2]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/venus.xls", "w");
    fp[3]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/earth.xls", "w");
    fp[4]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/mars.xls", "w");
    fp[5]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/jupiter.xls", "w");
    fp[6]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/saturn.xls", "w");
    fp[7]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/uranus.xls", "w");
    fp[8]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/neptune.xls", "w");
    fp[9]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/moon.xls", "w");
    fp[10]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v3.0)/难道是数据单位有问题？！/new planet.xls", "w");
    
    for (i=0; i<10; i++)
    {
        if (fp[i]==NULL)
            printf("Cannot find the file %d\n",i);
        else fprintf(fp[i], "%s\t%s\t%s\n","Time","X","Y");
    }

    double force[MAX][MAX][2];
    Pplanet solar_system[n];
    solar_system[0]=createPlanet("sun",MS*me,0,0,-1,0);
    solar_system[1]=createPlanet("mercury",0.0553*me,0.3871*AU,0,0,52716.3);
    solar_system[2]=createPlanet("venus",0.815*me,0.7233*AU,0,0,35242);
    solar_system[3]=createPlanet("earth",me,AU,0,0,29789);
    solar_system[4]=createPlanet("mars",0.1074*me,1.523*AU,0,0,25310);
    solar_system[5]=createPlanet("jupiter",317.834*me,5.202*AU,0,0,13413);
    solar_system[6]=createPlanet("saturnus",95.159*me,9.544*AU,0,0,9927.7);
    solar_system[7]=createPlanet("uranus",14.5*me,19.19*AU,0,0,6978);
    solar_system[8]=createPlanet("neptune",17.2*me,30.13*AU,0,0,5670.9);
    solar_system[9]=createPlanet("moon", 0.012*me, AU, rm, 1022, 0);

    double t=0;
    
    while(t<=100*YEAR)
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
                double f=GAV*m1*m2/(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
                force[i][j][0]=f*(x2-x1);
                force[i][j][1]=f*(y2-y1);
                force[j][i][0]=f*(x1-x2);
                force[j][i][1]=f*(y1-y2);
            }
        for (i=0;i<n;i++)
        {
            clean_acceleration(solar_system[i]);
            for (j=0;j<n;j++)
                if(i!=j)
                    calculate_acceleration(solar_system[i],force[i][j][0]/solar_system[i]->mass,force[i][j][1]/solar_system[i]->mass);
            calculate_velocity(solar_system[i]);
            calculate_coordination(solar_system[i]);
        }
        
        for (i=0; i<n; i++)
        {
            if (count%864000==0)
                fprintf(fp[i], "%lu\t%e\t%e\n",count/864000,solar_system[i]->x,solar_system[i]->y);
        }

        t+=step;
        count++;
    };
}