! Compilers for Parallel Systems
! 185.A64 WS 2013/14 H. Moritsch
! F90 generated  from EFL source

program jacobi

real, dimension(1:100,1:100) :: u
real, dimension(1:100,1:100) :: f
real, dimension(1:100,1:100) :: uhelp
real :: omega
real :: diff
real :: norm
integer :: i
integer :: j
integer :: iter
integer :: k

101 k = 1
102 if (k>0) then
103     omega = 0.900000
    else
104     if (k==0) then
105         omega = 0.750000
        else
106         omega = 0.500000
        end if
    end if
001 do iter = 1, 100
002     do i = 2, 100-1
003         do j = 2, 100-1
107             uhelp(i,j) = (1.000000-omega)*u(i,j)+omega*0.250000*(f(i,j)+u(i-1,j)+u(i+1,j)+u(i,j+1)+u(i,j-1))
            end do
        end do
004     do i = 2, 100-1
005         do j = 2, 100-1
108             diff = uhelp(i,j)-u(i,j)
109             norm = norm+diff*diff
            end do
        end do
006     do i = 2, 100-1
007         do j = 2, 100-1
110             u(i,j) = uhelp(i,j)
            end do
        end do
    end do
111 write(*,*) norm

end program jacobi
