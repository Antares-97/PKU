//
//  main.c
//  OpenGL Simulation(v3.0)
//
//  Created by MacBook air on 16/5/29.
//  Copyright © 2016年 LJZ. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#define GAV 66742800.0
#define MS 332608.0
#define me 5980000.0
#define Re 6371000000.0
#define AU 149597870000.0
#define Rm 1738000000.0
#define rm 3844000000
#define YEAR 31536000.0
#define step 10

GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPos[]={0.0f,0.0f,0.0f,1.0f};

struct planet
{
    char name[10];
    double mass;
    double radius;
    double x,y,vx,vy,ax,ay;
};
typedef struct planet *Pplanet;

static Pplanet solar_system[11];

Pplanet createPlanet(char name[10],double mass,double radius,double x,double y,double vx,double vy)
{
    Pplanet p=(Pplanet)malloc(sizeof(struct planet));
    if(p!=NULL)
    {
        int i;
        for(i=0;i<10;i++) p->name[i]=name[i];
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

void Display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5*10.2*AU,0.5*10.2*AU,-0.5*6.1*AU,0.5*6.1*AU,0.5*AU,-0.5*AU);                          //调用正投影视角
    
    //gluPerspective(90,2,1,10*AU);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(-1*5.3*AU,0,5.9*AU,0,0,0,0,0,1);                                                  //调用透视投影视角
    
    //glEnable(GL_LIGHTING);                                        // Setup and enable light 0设置并启用光0
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
    //glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
    //glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    //glEnable(GL_LIGHT0);                                          // Enable color tracking启用颜色跟踪
    //glEnable(GL_COLOR_MATERIAL);                          // Set Material properties to follow glColor values设置材料属性,遵循glColor值
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glPushMatrix();                                                 //太阳入栈
    glColor3ub(255, 34, 20);                                   //给太阳加红色
    glutSolidSphere(109.13*Re/43,20,20);             //绘制一个球体，调整其半径，即太阳
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();                                                 //水星入栈
    glColor3ub(68,233,10);                                   //给水星加绿色
    glutSolidTorus(Re/3.5,0.3880*AU,200,200);      //这个函数是绘制一个环，我们调整其半径，使其为水星轨道半径
    glTranslatef(solar_system[1]->x, solar_system[1]->y, 0.0f); //将水星的坐标传给绘图函数
    glutSolidSphere(0.38*Re,10,10);                     //绘制球体，调整半径，即水星
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();                                                 //金星入栈
    glColor3ub(254, 242, 58);                                 //给金星加金色
    glutSolidTorus(Re/3.5,0.7233*AU,10,100);        //绘制金星轨道
    glTranslatef(solar_system[2]->x, solar_system[2]->y, 0.0f);     //将金星的坐标传给绘图函数
    glutSolidSphere(0.95*Re,10,10);                     //绘制球体，即水星
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();                                                 //地球入栈
    glColor3ub(5,39,175);                                       //给地球加蓝色
    glutSolidTorus(Re/3.5,AU,10,100);                   //绘制地球轨道
    glTranslatef(solar_system[3]->x, solar_system[3]->y, 0.0f);          //将地球的坐标传给绘图函数
    glutSolidTorus(Re/3.5,4*rm,10,100);                 //绘制月球轨道，环的中心在地球所在位置
    glutSolidSphere(Re,10,10);                              //绘制地球
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();
    glColor3ub(198, 18, 35);
    glutSolidTorus(Re/4,1.523*AU,10,100);
    glTranslatef(solar_system[4]->x, solar_system[4]->y, 0.0f);
    glutSolidTorus(Re/3,3*Re,10,100);
    glutSolidSphere(0.53*Re,10,10);
    glPopMatrix();
    
    glPushMatrix();                                                 //水星入栈
    glColor3ub(157, 17, 243);                                   //给水星加绿色
    glutSolidTorus(Re/3.5,0.3880*AU,200,200);      //这个函数是绘制一个环，我们调整其半径，使其为水星轨道半径
    glTranslatef(solar_system[5]->x, solar_system[5]->y, 0.0f); //将水星的坐标传给绘图函数
    glutSolidSphere(0.38*Re,10,10);                     //绘制球体，调整半径，即水星
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();                                                 //金星入栈
    glColor3ub(198, 194, 68);                                 //给金星加金色
    glutSolidTorus(Re/3.5,0.7233*AU,10,100);        //绘制金星轨道
    glTranslatef(solar_system[6]->x, solar_system[6]->y, 0.0f);     //将金星的坐标传给绘图函数
    glutSolidSphere(0.95*Re,10,10);                     //绘制球体，即水星
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();                                                 //地球入栈
    glColor3ub(44, 187, 246);                                       //给地球加蓝色
    glutSolidTorus(Re/3.5,AU,10,100);                   //绘制地球轨道
    glTranslatef(solar_system[7]->x, solar_system[7]->y, 0.0f);          //将地球的坐标传给绘图函数
    glutSolidTorus(Re/3.5,4*rm,10,100);                 //绘制月球轨道，环的中心在地球所在位置
    glutSolidSphere(Re,10,10);                              //绘制地球
    glPopMatrix();                                                  //绘制完成，出栈
    
    glPushMatrix();
    glColor3ub(61, 19, 226);
    glutSolidTorus(Re/4,1.523*AU,10,100);
    glTranslatef(solar_system[8]->x, solar_system[8]->y, 0.0f);
    glutSolidTorus(Re/3,3*Re,10,100);
    glutSolidSphere(0.53*Re,10,10);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(solar_system[9]->x, solar_system[9]->y, 0.0f);
    glutSolidSphere(Re/2, 10, 10);
    glPopMatrix();
    
    //glPushMatrix();
    //glColor3ub(200, 200, 200);
    //glTranslatef(solar_system[10]->x, solar_system[10]->y, 0.0f);
    //glutSolidSphere(Re, 10, 10);
    //glPopMatrix();
    
    glFlush();                      //强行刷新缓冲，保证绘图命令被执行
    glutSwapBuffers();       //我们采用双缓冲区绘图，因此要交换缓冲区
}

void Idle()
{
    int k=10;
    double x1,y1,m1,r1,v1x,v1y,x2,y2,m2,r2,v2x,v2y;
    double force[k][k][2];
    int n,i,j;
    for(n=0;n<10000;n++)
    {
        for(i=0;i<k;i++)
            for(j=i+1;j<k;j++)
            {
                x1=solar_system[i]->x;
                y1=solar_system[i]->y;
                x2=solar_system[j]->x;
                y2=solar_system[j]->y;
                
                m1=solar_system[i]->mass;
                m2=solar_system[j]->mass;
                
                r1=solar_system[i]->radius/1000;
                r2=solar_system[j]->radius/1000;
                
                v1x=solar_system[i]->vx;
                v1y=solar_system[i]->vy;
                v2x=solar_system[j]->vx;
                v2y=solar_system[j]->vy;
                
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
                    
                    //solar_system[i]->vx=(m1*v1x+m2*v2x)/(m1+m2);
                    //solar_system[i]->vy=(m1*v1y+m2*v2y)/(m1+m2);
                    //solar_system[j]->vx=solar_system[i]->vx;
                    //solar_system[j]->vy=solar_system[i]->vy;
                }
                
            }
        for (i=1;i<k;i++)
        {
            clean_acceleration(solar_system[i]);
            for (j=0;j<k;j++)
                if(i!=j)
                    calculate_acceleration(solar_system[i],force[i][j][0]/solar_system[i]->mass,force[i][j][1]/solar_system[i]->mass);
            calculate_coordination(solar_system[i]);
            calculate_velocity(solar_system[i]);
        }
    }
    Display();
}

void create_solar_system()
{
    solar_system[0]=createPlanet("sun",1*MS*me,109.13*Re,0.0,0.0,0.0,0);
    solar_system[1]=createPlanet("mercury",0.0553*me,0.38*Re,0.3871*AU,0.0,0.0,52316.3);
    solar_system[2]=createPlanet("venus",0.815*me,0.95*Re,0.7233*AU,0.0,0.0,35242.0);
    solar_system[3]=createPlanet("earth",me,Re,AU,0.0,0.0,29789.0);
    solar_system[4]=createPlanet("mars",0.1074*me,0.53*Re,1.523*AU,0.0,0.0,25310.0);
    solar_system[5]=createPlanet("mercury2",0.0553*me,0.38*Re,-0.3871*AU,0.0,0.0,52316.3);      //水星的镜像行星
    solar_system[6]=createPlanet("venus2",0.815*me,0.95*Re,-0.7233*AU,0.0,0.0,35242.0);             //金星的镜像行星
    solar_system[7]=createPlanet("earth2",me,Re,-AU,0.0,0.0,29789.0);                                           //地球的镜像行星
    solar_system[8]=createPlanet("mars2",0.1074*me,0.53*Re,-1.523*AU,0.0,0.0,25310.0);              //火星的镜像行星
    solar_system[9]=createPlanet("moon",0.012*me,0.27*Re,AU,4*rm,(-1)*500.0,29789.0);
    //solar_system[5]=createPlanet("jupiter",317.834*me,11.19*Re,5.202*AU,0.0,0.0,13413);
    //solar_system[6]=createPlanet("saturn",95.159*me,9.41*Re,9.544*AU,0.0,0.0,9927.7);
    //solar_system[7]=createPlanet("uranus",14.5*me,4.01*Re,19.19*AU,0.0,0.0,6978.0);
    //solar_system[8]=createPlanet("neptune",17.2*me,3.88*Re,30.13*AU,0.0,0.0,5670.9);
    //solar_system[9]=createPlanet("moon",0.012*me,0.27*Re,AU,4*rm,(-1)*500.0,29789.0);
    //solar_system[10]=createPlanet("2earth", me, Re, -1*AU, 0, 0, 29789.0);
}

int main(int argc, char *argv[])
{
    create_solar_system();                                              //创建太阳系
    
    glutInit(&argc, argv);                                                //初始化
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //设置显示方式，使用RGB类型显示颜色，使用双缓冲区
    glutInitWindowPosition(0, 0);                                  //设置窗口位置
    glutInitWindowSize(1450,900);                               //设置窗口大小
    glutCreateWindow("solar_system");                        //创建显示窗口，设置窗口名称
    glutDisplayFunc(&Display);                                     //调用绘图函数
    glutIdleFunc(&Idle);                                                 //在CPU空闲时，调用Idle函数
    glutMainLoop();                                                       //消息循环，显示窗口
    
    return 0;
}
