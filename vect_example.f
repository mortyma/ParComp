      program vect

      parameter(n=20,m=30)
      real a(n,m),b(n,m),c(n,m),d(n,m)
      do 11 i=1,n
         do 12 j=1,n
            a(i,j)=100.0*i+j+0.1
            b(i,j)=100.0*i+j+0.2
            c(i,j)=100.0*i+j+0.3
            d(i,j)=100.0*i+j+0.4
12        continue
11     continue

      do 21 i=2,n-1
         do 22 j=3,m-4
            c(i,j)=a(i,j)*b(i,j)
            a(i+1,j+1)=c(i,j-2)/2+c(i-1,j)*3
            d(i,j)=d(i-1,j-1)+1
            b(i,j+4)=d(i,j)-1
22       continue
21    continue

      do 31 i=1,n
         do 32 j=1,n
            write (*,*),'a',i,j,a(i,j)
            write (*,*),'b',i,j,b(i,j)
            write (*,*),'c',i,j,c(i,j)
            write (*,*),'d',i,j,d(i,j)
32        continue
31     continue

      end
