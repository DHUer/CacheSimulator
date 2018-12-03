#ifndef LINE_H
#define LINE_H

class Line
{
  public:
    unsigned tag;
    bool isValid;
    bool isDirty;
    // data field, we do not really need it in this lab.
    Line(unsigned tag = 0, bool isValid = false, bool isDirty = false) : tag(tag), isValid(isValid), isDirty(isDirty) {}
};

#endif