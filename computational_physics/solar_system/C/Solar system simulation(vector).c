#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 11
#define GAV 66742800.0
#define MS 332608.0
#define Re 6378000.0
#define me 5980000.0
#define AU 149597870000.0
#define rm 384400000
#define YEAR 31536000.0
#define step 1

struct vec
{
    double vector_x;
    double vector_y;
    double module;
    double angle;
};
typedef struct vec * vector;

vector addvector(vector vec1,vector vec2)      //vec1加上vec2
{
    vector v=(vector)malloc(sizeof(struct vec));
    v->vector_x=vec1->vector_x+vec2->vector_x;
    v->vector_y=vec1->vector_y+vec2->vector_y;
    return v;
}

vector minusvector(vector vec1,vector vec2)     //vec1减去vec2
{
    vector v=(vector)malloc(sizeof(struct vec));
    v->vector_x=vec1->vector_x-vec2->vector_x;
    v->vector_y=vec1->vector_y-vec2->vector_y;
    return v;
}

vector mutiply(double b, vector vec)   //vec与数 b 的数乘
{
    vector v=(vector)malloc(sizeof(struct vec));
    v->vector_x=b*vec->vector_x;
    v->vector_y=b*vec->vector_y;
    return v;
}

double resultdot(vector vec1,vector vec2)   //vec1与vec2点乘的结果
{
    double dotreseult;
    dotreseult=vec1->vector_x*vec2->vector_x+vec1->vector_y*vec2->vector_y;
    return dotreseult;
}

vector normalvector(vector vec,vector n)    //取vec平行于单位向量n的分量
{
    vector normalvector=(vector)malloc(sizeof(struct vec));
    normalvector=mutiply(resultdot(vec, n), n);
    return normalvector;
}

vector tangentvector(vector vec,vector n)    //取vec垂直于单位向量n的分量
{
    vector tangentvector=(vector)malloc(sizeof(struct vec));
    tangentvector=minusvector(vec, normalvector(vec, n));
    return tangentvector;
}

struct planet
{
    char * name;
    double mass;
    double radius;
    vector r,v,a;
};
typedef struct planet *Pplanet;

Pplanet createPlanet(char * name,double mass,double r,double x,double y,double vx,double vy)
{
    Pplanet p=(Pplanet)malloc(sizeof(struct planet));
    if(p!=NULL)
    {
        p->name=(char *)malloc(sizeof(char)*10);
        strcpy(p->name, name);
        p->mass=mass;
        p->radius=r;
        p->r->vector_x=x;
        p->r->vector_y=y;
        p->v->vector_x=vx;
        p->v->vector_y=vy;
    }
    else
        printf("create planet failed");
    return p;
}

void clean_acceleration(Pplanet p)
{
    p->a->vector_x=0;
    p->a->vector_y=0;
}

void calculate_acceleration(Pplanet p,vector gravity)
{
    p->a=mutiply(1/p->mass, gravity);
}

void calculate_velocity(Pplanet p)
{
    p->v=addvector(p->v, mutiply(step, p->a)) ;
}

void calculate_coordination(Pplanet p)
{
    p->r=addvector(p->r, addvector(mutiply(step, p->v), mutiply(0.5*step*step, p->a)));
}

int main()
{
    int n=10;
    int i,j;
    unsigned long int count=0;
    FILE *fp[10];
    fp[0]=NULL;
    fp[1]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/mercury.xls","w");
    fp[2]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/venus.xls","w");
    fp[3]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/earth.xls","w");
    fp[4]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/mars.xls","w");
    fp[5]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/jupiter.xls","w");
    fp[6]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/saturn.xls","w");
    fp[7]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/uranus.xls","w");
    fp[8]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/neptune.xls","w");
    fp[9]=fopen("/Volumes/LJZ的外置硬盘/LJZ PKU/计概/数算/Solar System Simulation/solar_system(v4.0)/solar_system(v4.0)/moon.xls","w");
    for (i=1; i<9; i++)
    {
        if (fp[i]==NULL)
            printf("Cannot find the file %d\n",i);
        else fprintf(fp[i], "%s\t%s\t%s\n","Time","X","Y");
    }
    
    vector ** gravity;
    gravity=(vector **)malloc(sizeof(struct vec)*MAX);
    for(i=0;i<MAX;i++)
    {
        gravity[i]=(vector *)malloc(sizeof(struct vec)*MAX);
    }
    
    Pplanet solar_system[n];
    solar_system[0]=createPlanet("sun",MS*me,109.13*Re,0.0,0.0,0.0,0);
    solar_system[1]=createPlanet("mercury",0.0553*me,0.38*Re,0.3871*AU,0.0,0.0,52716.3);
    solar_system[2]=createPlanet("venus",0.815*me,0.95*Re,0.7233*AU,0.0,0.0,35242);
    solar_system[3]=createPlanet("earth",me,Re,AU,0.0,0.0,29789);
    solar_system[4]=createPlanet("mars",0.1074*me,0.53*Re,1.523*AU,0.0,0.0,25310);
    solar_system[5]=createPlanet("jupiter",317.834*me,11.19*Re,5.202*AU,0.0,0.0,13413);
    solar_system[6]=createPlanet("saturnus",95.159*me,9.41*Re,9.544*AU,0.0,0.0,9927.7);
    solar_system[7]=createPlanet("uranus",14.5*me,4.01*Re,19.19*AU,0.0,0.0,6978);
    solar_system[8]=createPlanet("neptune",17.2*me,3.88*Re,30.13*AU,0.0,0.0,5670.9);
    solar_system[9]=createPlanet("moon",0.012*me,0.27*Re,AU,rm,1022,0.0);
    double t=0;
    
    while(t<=YEAR)
    {
        for(i=0;i<n;i++)
            for(j=i+1;j<n;j++)
            {
                double x1=solar_system[i]->r->vector_x;
                double y1=solar_system[i]->r->vector_y;
                double x2=solar_system[j]->r->vector_x;
                double y2=solar_system[j]->r->vector_y;
                double m1=solar_system[i]->mass;
                double m2=solar_system[j]->mass;
                double f=GAV*m1*m2/(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
                double x21=(x2-x1)/sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                double y21=(y2-y1)/sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                double x12=(-1)*x21;
                double y12=(-2)*y21;

                gravity[i][j]->vector_x=f*x21;
                gravity[i][j]->vector_y=f*y21;
                gravity[j][i]->vector_x=f*x12;
                gravity[j][i]->vector_y=f*y12;
                
                if ( (solar_system[i]->radius+solar_system[j]->radius)>=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)) )
                {
                    vector n21=malloc(sizeof(struct vec));
                    vector n12=malloc(sizeof(struct vec));
                    vector vc=malloc(sizeof(struct vec));
                    vector v1r=malloc(sizeof(struct vec));
                    vector v1rn=malloc(sizeof(struct vec));
                    vector v1rt=malloc(sizeof(struct vec));
                    vector v2r=malloc(sizeof(struct vec));
                    vector v2rn=malloc(sizeof(struct vec));
                    vector v2rt=malloc(sizeof(struct vec));

                    vc=addvector(mutiply(m1/(m1+m2), solar_system[i]->v), mutiply(m2/(m1+m2), solar_system[j]->v));             //质心速度
                    v1r=minusvector(solar_system[i]->v, vc);                  //1相对质心速度
                    v2r=minusvector(solar_system[j]->v, vc);                 //2相对质心速度
                    
                    n21->vector_x=x21;
                    n21->vector_y=y21;                                                  //2对1的单位矢量
                    n12->vector_x=x12;
                    n12->vector_y=y12;                                                  //1对2的单位矢量
                    
                    v1rn=normalvector(v1r, n21);                                   //1相对速度法向分量
                    v1rt=tangentvector(v1r, n21);                                  //1相对速度切向分量
                    v2rn=normalvector(v2r, n12);                                   //2相对速度法向分量
                    v2rt=tangentvector(v2r, n12);                                  //2相对速度切向分量
                    
                    v1rn=mutiply(-1, v1rn);                                            //法向分量相反
                    v2rn=mutiply(-1, v2rn);                                            //切向分量不变
                    
                    v1r=addvector(v1rn, v1rt);
                    v2r=addvector(v2rn, v2rt);                                       //修改碰撞后的相对质心速度
                    
                    solar_system[i]->v=addvector(v1r, vc);
                    solar_system[j]->v=addvector(v2r, vc);                   //修改碰撞后的速度（加上质心速度）
                }
            }
        for (i=1;i<n;i++)
        {
            clean_acceleration(solar_system[i]);
            for (j=0;j<n;j++)
            {
                if(i!=j)
                    calculate_acceleration(solar_system[i],gravity[i][j]);
            }
            
            calculate_velocity(solar_system[i]);
            calculate_coordination(solar_system[i]);
        }
        
        for (i=1; i<10; i++)
        {
            if (count%86400==0)
                fprintf(fp[i], "%lu\t%e\t%e\n",count/86400,solar_system[i]->r->vector_x,solar_system[i]->r->vector_y);
        }
        
        t+=step;
        count++;
    };
}