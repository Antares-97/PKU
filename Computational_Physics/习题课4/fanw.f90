program fanw
implicit none
external DSYEV
integer i,j,k,info,info1
integer,parameter::n=500
integer,parameter::m=10
double precision q1(n,n),qq(n,n),qm(m,m),beta(m+1),alpha(m),q(n,m+2),w(n),v(n),b(n),tr(m,m),di(m),wor(m)
double precision bb,vv
double precision tes(3,3),td(3),wort(9),worn(n*n),dr(n)
!data tes /1,2,3,2,1,3,3,3,6/
!do i=1,3
!   do j=1,3
!      write(*,*)i,j,tes(i,j)
!   end do
!end do
open(6,file="5004outq.txt",status="new")
open(7,file="5004value.txt",status="new")
call random_seed()
call random_number(q1)
call random_number(b)
!write(6,"(5F10.3)") ((q1(i,j),j=1,n),i=1,n)
do i=1,n
    do j=1,n
        if(i.le.j)then
          qq(i,j)=0.0
          do k=1,n
              qq(i,j)=q1(i,k)*q1(j,k)+qq(i,j)
          end do
        else
          qq(i,j)=0
        end if
   end do
end do
do i=1,n     
   do j=1,n
      if(i.gt.j)then
        qq(i,j)=qq(j,i)
      end if
   end do
end do
write(6,"(20F10.3)") ((qq(i,j),j=1,n),i=1,n)
do i=1,n
   q(i,1)=0.0
   bb=b(i)*b(i)+bb
!   alpha(i)=0.0
end do
do i=1,m
   alpha(i)=0.0
end do
do i=1,n
   q(i,2)=b(i)/sqrt(bb)
   beta(i)=0.0
end do
beta(1)=0.0
!do i=1,n
!   write(6,"(2F10.3)")q(i,1),q(i,2)
!end do
do i=1,m
   vv=0.0
!   write(6,*)"to do" ,i
   do j=1,n
      w(j)=0.0
      do k=1,n
         w(j)=qq(j,k)*q(k,i+1)+w(j)
      end do
 !     write(6,*)"w(j)",w(j)
   end do
   do k=1,n
      alpha(i)=w(k)*q(k,i+1)+alpha(i)
   end do
!   write(6,*)"doing",i
   do k=1,n
  !    write(6,*)v(k)
      v(k)=w(k)-beta(i)*q(k,i)-alpha(i)*q(k,i+1)
  !    write(6,*)v(k)
      vv=v(k)*v(k)+vv
   end do
   beta(i+1)=0.0
   do k=1,n
      q(k,i+2)=v(k)/sqrt(vv)
      beta(i+1)=q(k,i+2)*v(k)+beta(i+1)
   end do
!   write(6,*)i,vv
end do
!write(6,*)"I'm fine!"
!do i=1,m
!   write(6,"(2F10.3)")beta(i),alpha(i)
!end do
do i=1,m
   do j=1,m
      if(j.eq.i)then
        tr(i,j)=alpha(i)
      else if(j.eq.i+1)then
        tr(i,j)=beta(i+1)
      else if(j.eq.i-1)then
        tr(i,j)=beta(i)
      else
        tr(i,j)=0.0
      end if
   end do
end do
write(6,"(10F10.3)") ((tr(i,j),j=1,m),i=1,m)
call DSYEV('N','U',m,tr,m,di,wor,m*m,info)
call DSYEV('N','U',n,qq,n,dr,worn,n*n,info1)
!call DSYEV('N','U',3,tes,3,td,wort,9,info)
write(6,*)"write the eigenvalue"
do i=1,m
   write(6,*)di(i)
end do
write(6,*)"info is: ",info
write(7,*)"info is: ",info1
do i=1,n
   write(7,*)dr(i)
end do
end 
