c**********************************************************
c
      program test
c
c Trivial test program to demonstrate the use of
c lcgetname and lccreatecollectionvec from Fortran
c 
c H. Vogt  30/09/2003
c**********************************************************
      implicit none

C This may be architecture dependent
#define PTRTYPE INTEGER


      character*20 lcgetname,  result, orig
      integer i, lenocc, status, lccreatecollectionvec

      integer ncols
      parameter (ncols=8)
      character*20 typenames(ncols)
      data typenames /'MCPARTICLE', 'SIMCALORIMETERHIT', 
     &                'CALORIMETERHIT', 'SIMTRACKERHIT', 'TPCHIT', 
     &                'LCFLOATVEC' , 'LCINTVEC' , '****'           /

      do i = 1,ncols
        orig = typenames(i)
        result = lcgetname( typenames(i) )
        print '(a,a,a,a)', 'String for ',
     &        orig(1:lenocc(orig)),
     &        ' is ', result(1:lenocc(result))
        if (result(1:lenocc(result)) .ne. 'undefined')  then
          status = lccreatecollectionvec ( typenames(i) )
          print '(a,z8)', 'Status for LCCreateCollectionVec is: '
     &                  , status
        endif
      enddo
      stop

      end

      INTEGER FUNCTION LENOCC (CHV)
C
C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
C
C-    Find last non-blank character in CHV

      CHARACTER    CHV*(*)

      N = LEN(CHV)

      DO 17  JJ= N,1,-1
      IF ((CHV(JJ:JJ).NE.' ').AND.(CHV(JJ:JJ).NE.CHAR(0))) GO TO 99
   17 CONTINUE
      JJ = 0

   99 LENOCC = JJ
      RETURN
      END
*
