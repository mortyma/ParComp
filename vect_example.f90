      program vect

      integer, parameter :: n = 20
      integer, parameter :: m = 30
      integer :: i,j
      real, dimension(1:n,1:m) :: a,b,c,d

      do i=1,n
         do j=1,n
            a(i,j)=100.0*i+j+0.1
            b(i,j)=100.0*i+j+0.2
            c(i,j)=100.0*i+j+0.3
            d(i,j)=100.0*i+j+0.4
         end do
      end do

      do i = 2,n-1
        d(i,3:m-4) = d(i-1,2:m-5) + 1
      end do

      b(2:n-1,7:m) = d(2:n-1,3:m-4) - 1

      do i = 2,n-1
        c(i,3:m-4) = a(i,3:m-4) * b(i,3:m-4)
        a(i+1,4:m-3) = c(i,1:m-6)/2 + 3*c(i-1,3:m-4)
      end do

      do i=1,n
         do j=1,n
            write (*,*),'a',i,j,a(i,j)
            write (*,*),'b',i,j,b(i,j)
            write (*,*),'c',i,j,c(i,j)
            write (*,*),'d',i,j,d(i,j)
         end do
      enddo
    
      end program  
