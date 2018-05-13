      PROGRAM MAIN
      INTEGER N, X
      EXTERNAL SUB1
      COMMON /GLOBALS/ N
      X = 0
      PRINT *, 'Enter number of repeats'
      READ (*,*) N
      CALL SUB1(X,SUB1)
      END

      SUBROUTINE SUB1(X,DUMSUB)
      INTEGER N, X
      EXTERNAL DUMSUB
      COMMON /GLOBALS/ N
      IF(X .LT. N)THEN
        X = X + 1
        PRINT *, 'x = ', X
        CALL DUMSUB(X,DUMSUB)
      END IF
      END
