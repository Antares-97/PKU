program wkb1
implicit none
integer,parameter::d=43
integer,parameter::tt=50
integer,parameter::dd=d+(d-1)*(tt-1)
real,parameter ::h=6.5820944e-22 !the parameter h/2pi(MeV.ns)
real,parameter ::pi=3.1415926
real,parameter ::err1=0.008!0.008 !the inital scaling for energy 
real,parameter ::err2=0.10 !the sommerfeld equation
real,parameter ::db=0.001 !the inital deformation of the beta2
real,parameter ::dbeta=0.05 !the step of the deformation 
real,parameter ::tr=50
integer i,j,k
real*8 a,b,c,pol
real*8 beta(d),energy(d),energy2(dd),mass2(dd),aa(10)
real*8 mass(d)
real iniene(d)
real*8 integ1v,integ1e,sft
real integ2e(d)
integer ai(10),beta1(dd)
real*8 e,integ3,integ2
integer emax,emid,emin
real t1(10),t3(10)
integer t2(10),t4(10)
open(10,file="energy.txt",status="old")
do i=1,d
  read(10,"(F10.5)") energy(i)
end do
close(10,status="keep")
open(15,file="crmasqtot.txt",status="old")
do i=1,d
  read(15,"(F16.12)") mass(i)
end do
close(15,status="keep")
!open(12,file="energy2.txt",status="old")
!do i=1,d
!   write(*,"(F16.12)") mass(i)
!end do
!use the sommerfeld equation to find the inital energy and the integration a,b
data t1 /10*0.0/
data t2 /10*0/
data t3 /10*0.0/
data t4 /10*0/
data aa /10*0.0/
data energy2 /dd*0.0/
call smoothpara(energy,energy2,d,dd,tt,tr)
!call smoothakima(energy,energy2,d,dd,tt,tr)
do i=1,dd
  write(12,"(F12.5)")energy2(i)
end do
call smoothpara(mass,mass2,d,dd,tt,tr)
!call smoothakima(mass,mass2,d,dd,tt,tr)
!do i=1,dd
!  write(*,"(F12.5)")mass2(i)
!end do
do i=1,dd
  mass2(i)=0.65*mass2(i)
end do
!call search(energy2,dd,t1,t2,t3,t4,tt)
call searchnew(energy2,dd,t1,t2,t3,t4,tt)
!do i=1,10
!  write(*,"(F12.5,I4,I2)")t1(i),t2(i),i
!  write(*,"(F12.5,I4)")t3(i),t4(i)
!end do
!call sommerfeld(energy2,mass2,dd,t4(2),t2(3),t4(5),e,i,emin,emid,pi,err1,err2,db)
call sommerfeld(energy2,mass2,dd,0,t2(1),t4(2),e,i,emin,emid,pi,err1,err2,db)
!call sommerfeld(energy2,mass2,dd,t4(1),t2(2),t4(2),e,i,emin,emid,pi,err1,err2,db)
write(*,"(a,I3)")"t2(2)=",t2(2)
write(*,"(a,I3)")"t2(1)=",t2(1)
write(*,"(a,I3)")"i=",i
write(*,"(a,I5,a,I5)")"emin= ",emin,"emid= ",emid
write(*,"(a,F15.8)")"energy2(t2(1))=",energy2(t2(1))
write(*,"(a,F12.5)")"energy2(t2(2))=",energy2(t2(2))
write(*,"(a,F12.5)")"energy2(t4(2))=",energy2(t4(2))-energy2(t2(1))
write(*,"(a,F12.5)")"energy2(t4(3))=",energy2(t4(3))-energy2(t2(1))
write(*,"(a,F12.5)")"energy2(emid)=",energy2(emid)-energy2(t2(1))
write(*,"(a,F12.5,a,F12.5,a,F12.5)")"e=",e,"energy2(emin)=",energy2(emin),"energy2(emid)=",energy2(emid)
write(*,"(a,F12.5,a,F12.5,a,F12.5)")"e=",e,"energy2(emin)=",energy2(emin+1),"energy2(emid)=",energy2(emid-1)
!do j=i,emin
!write(*,"(a,F15.8)")"energy2=",e-energy2(j)
!end do
!write(*,"(a,F12.5)")"the error is",integ1v-pi/2.0
!calulate the integration 
!integ2=0.005*sqrt(2.0*mass2(i+1)/abs(e-energy(i+1)))/2.0
integ3=0
do j=i+1,emin
   integ2=integ2+0.005*(sqrt(2.0*mass2(j+1)/abs(e-energy2(j+1)))+sqrt(2.0*mass2(j)/abs(e-energy2(j))))/2.0
!   write(*,"(a,F10.5)") "integ2",db*sqrt(2.0*mass2(j)/abs(e-energy2(j)))
end do
do j=emin+1,emid-1
!   integ3=integ3+db*(sqrt(2.0*mass2(j-1)*abs(energy2(j-1)-e))+sqrt(2.0*mass2(j)*abs(energy2(j)-e)))/2.0
   integ3=integ3+db*(sqrt(mass2(j+1)*abs(energy2(j+1)-e))+sqrt(mass2(j)*abs(energy2(j)-e)))/2.0
!    integ3=integ3+db*(sqrt(2.0*mass2(j+1)*abs(energy2(j+1)-0.9))+sqrt(2.0*mass2(j)*abs(energy2(j)-0.9)))/2.0
end do
sft=integ2*(1+exp(2*integ3))*h
write(*,"(a,F12.5)") "integ2=",integ2
write(*,"(a,F10.5)") "integ3=",integ3
write(*,"(a,E15.6)") "the lifetime of SF is:",sft
stop
end    


subroutine smoothpara(a,c,d,dd,tt,tr)
implicit none
  real*8 a(d),beta(d)
  real*8 c(dd),beta1(dd)
  real*8 a1,b1,c1
  integer i,j,d,dd,tt
  real tr
  do i=1,dd
    beta1(i)=(i-1)
  end do
  do i=1,d
    beta(i)=(i-1)
  end do
  do i=2,d-1,2
    a1=(-a(i-1)+2*a(i)-a(i+1))/(-2*tr**2)
    b1=((beta(i+1)**2-beta(i)**2)*a(i-1)+(beta(i-1)**2-beta(i+1)**2)*a(i)+(beta(i)**2-beta(i-1)**2)*a(i+1))/(-2.0*tr)
    c1=(-beta(i)*beta(i+1)*a(i-1)+beta(i-1)*beta(i+1)*2*a(i)-beta(i-1)*beta(i)*a(i+1))/(-2.0)
    c((i-2)*tt+1)=a(i-1)
  do j=1,tt
    c((i-2)*tt+j+1)=a1*beta1((i-2)*tt+j+1)**2+b1*beta1((i-2)*tt+j+1)+c1
  end do
  c((i-1)*tt+1)=a(i)
  do j=1,tt
    c((i-1)*tt+j+1)=a1*beta1((i-1)*tt+j+1)**2+b1*beta1((i-1)*tt+j+1)+c1
  end do
    c(i*tt+1)=a(i+1)
  end do
  return
end  


subroutine search(a,dd,t1,t2,t3,t4,tt)
implicit none
real*8 a(dd)
real t1(10),t3(10)
integer t2(10),t4(10)
integer i,j,tt,dd
do i=2,dd-1
   if(((a(i)-a(i-1)).le.0.0).and.((a(i)-a(i+1)).le.0.0))then
     do j=1,tt
     if(t1(j).eq.0.0)then
       t1(j)=a(i)
       t2(j)=i
       exit
     end if
     end do
   elseif(((a(i)-a(i-1)).ge.0.0).and.((a(i)-a(i+1)).ge.0.0))then
     do j=1,tt
     if(t3(j).eq.0.0)then
       t3(j)=a(i)
       t4(j)=i
       exit
     end if
     end do
   end if
end do
return
end
subroutine searchnew(a,dd,t1,t2,t3,t4,tt)
implicit none
real*8 a(dd)
real t1(10),t3(10),yd(4),yu(4)
integer t2(10),t4(10),ydi(4),yui(4)
data ydi /4*0/
data yui /4*0/
integer i,j,tt,dd
do i=tt,dd-tt,tt
   if(((a(i)-a(i-tt)).le.0.0).and.((a(i)-a(i+tt)).le.0.0))then
     do j=1,4
     if(ydi(j).eq.0.0)then
!       yd(j)=a(i)
       ydi(j)=i
       exit
     end if
     end do
   elseif(((a(i)-a(i-tt)).ge.0.0).and.((a(i)-a(i+tt)).ge.0.0))then
     do j=1,4
     if(yui(j).eq.0.0)then
!       yu(j)=a(i)
       yui(j)=i
       exit
     end if
     end do
   end if
end do
!do j=1,4
!   write(*,*) ydi(j)
!end do 
!do j=1,4
!   write(*,*) yui(j)
!end do

do i=2,dd-1
   if(((a(i)-a(i-1)).le.0.0).and.((a(i)-a(i+1)).le.0.0))then
     do j=1,10
     if(t1(j).eq.0.0 .and. abs(i-ydi(j))<2*tt)then
       t1(j)=a(i)
       t2(j)=i
       exit
     end if
     end do
   elseif(((a(i)-a(i-1)).ge.0.0).and.((a(i)-a(i+1)).ge.0.0))then
     do j=1,10
     if(t3(j).eq.0.0 .and. abs(i-yui(j))<2*tt)then
       t3(j)=a(i)
       t4(j)=i
       exit
     end if
     end do
   end if
end do
return 
end
subroutine sommerfeld(a,b,dd,c,d,f,e,i,emin,emid,pi,err1,err2,db)
implicit none
real*8 integ1v,e,aa(10),a(dd),b(dd)
real pi,err1,err2,db
integer i,j,k,dd,ai(10),c,d,f,emid,emin
integ1v=0;
data ai /10*0/
data aa /10*0.0/
do i=c,d-1
  e=a(i)
  do j=d+1,dd
    do k=2,10
      if((abs(a(j)-e).le.err1).and.(aa(k).eq.0))then
        aa(k)=a(j);
        ai(k)=j;
        exit
      end if
    end do
  end do
  if(aa(2).eq.0)then   !can not find same energy point exit
    cycle
  end if
  aa(1)=e  !k start from 2
  ai(1)=i
  if(ai(3).le.f)then    !f is energy minimum
    emin=ai(3)
    emid=ai(4)
  else
    emin=ai(2)
    emid=ai(3)
  end if
!  do k=1,10     
!    write(*,"(a,F15.5)") "aa=",aa(k)
!    write(*,"(a,I5)") "ai=",ai(k)
!  end do
!  write(*,"(a,I5)")"emin= ",emin
!  write(*,"(a,I5)")"emid= ",emid
  do k=ai(1)+1,emin
    integ1v=integ1v+(sqrt(2*b(k-1)*abs(a(k-1)-e))+sqrt(2*b(k)*abs(a(k)-e)))*db/2.0
  end do
!  write(*,"(a,F12.5)")"integ1v=",integ1v-pi/2.0
  if(abs(integ1v-pi/2.0).le.err2) exit
  do k=1,10  !recover aa and ai 
     aa(k)=0.0
     ai(k)=0
  end do
  integ1v=0
end do
write(*,"(a,F12.5)")"the error is",integ1v-pi/2.0
return 
end

subroutine smoothakima(a,c,d,dd,tt,tr)
implicit none
  real*8 a(d),beta(d)
  real*8 c(dd),beta1(dd)
  real*8 a1,b1,c1
  integer i,j,d,dd,tt
  real tr,m1,m2,m3,m4,m5,p0,p1,p2,p3,t3,t4
  do i=1,dd
    beta1(i)=(i-1)
  end do
  do i=1,d
    beta(i)=(i-1)
  end do
  do i=2,2
    a1=(-a(i-1)+2*a(i)-a(i+1))/(-2*tr**2)
    b1=((beta(i+1)**2-beta(i)**2)*a(i-1)+(beta(i-1)**2-beta(i+1)**2)*a(i)+(beta(i)**2-beta(i-1)**2)*a(i+1))/(-2.0*tr)
    c1=(-beta(i)*beta(i+1)*a(i-1)+beta(i-1)*beta(i+1)*2*a(i)-beta(i-1)*beta(i)*a(i+1))/(-2.0)
    c((i-2)*tt+1)=a(i-1)
  do j=1,tt
    c((i-2)*tt+j+1)=a1*beta1((i-2)*tt+j+1)**2+b1*beta1((i-2)*tt+j+1)+c1
  end do
  c((i-1)*tt+1)=a(i)
  do j=1,tt
    c((i-1)*tt+j+1)=a1*beta1((i-1)*tt+j+1)**2+b1*beta1((i-1)*tt+j+1)+c1
  end do
    c(i*tt+1)=a(i+1)
  end do
  do i=3,d-3
    p0=a(i)
    m3=(a(i+1)-a(i))/0.05
    m4=(a(i+2)-a(i+1))/0.05
    m2=(a(i)-a(i-1))/0.05
    m1=(a(i-1)-a(i-2))/0.05
    m5=(a(i+3)-a(i+2))/0.05
    t3=(abs(m4-m3)*m2+abs(m2-m1)*m2)/(abs(m4-m3)+abs(m2-m1))
    t4=(abs(m5-m4)*m3+abs(m3-m2)*m3)/(abs(m5-m4)+abs(m3-m2))
    p1=t3
    p2=(3*(a(i+1)-a(i))/0.05-2*t3-t4)/0.05
    p3=(t3+t4-2*(a(i+1)-a(i))/0.05)/0.05**2
    c((i-1)*tt+1)=a(i)
    do j=1,tt
      c((i-1)*tt+j)=p0+p1*0.05/tt*j+p2*(0.05*j/tt)**2+p3*(0.05*j/tt)**3
    end do
  end do
  do i=d-1,d-1
    a1=(-a(i-1)+2*a(i)-a(i+1))/(-2*tr**2)
    b1=((beta(i+1)**2-beta(i)**2)*a(i-1)+(beta(i-1)**2-beta(i+1)**2)*a(i)+(beta(i)**2-beta(i-1)**2)*a(i+1))/(-2.0*tr)
    c1=(-beta(i)*beta(i+1)*a(i-1)+beta(i-1)*beta(i+1)*2*a(i)-beta(i-1)*beta(i)*a(i+1))/(-2.0)
    c((i-2)*tt+1)=a(i-1)
  do j=1,tt
    c((i-2)*tt+j+1)=a1*beta1((i-2)*tt+j+1)**2+b1*beta1((i-2)*tt+j+1)+c1
  end do
  c((i-1)*tt+1)=a(i)
  do j=1,tt
    c((i-1)*tt+j+1)=a1*beta1((i-1)*tt+j+1)**2+b1*beta1((i-1)*tt+j+1)+c1
  end do
    c(i*tt+1)=a(i+1)
  end do
  return
end

