/* Simple class to record a grid location. Member variables are public to allow easy access.*/
class Loc {
public:
  Loc(int r, int c) { row = r; col = c; }
  int row, col;
};