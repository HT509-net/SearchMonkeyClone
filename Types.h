#ifndef TYPES_H
#define TYPES_H

enum TimeType{AccessedTime, ModifiedTime, CreatedTime};

QString LowBoundFilterString(const TimeType timeType);

enum Case {SENSITIVE, INSENSITIVE};
// in ui stacked widget index 0 is horiz layout, 1 is vert layout
enum Layout {HORIZONTAL, VERTICAL};
enum State {QUICK, ADVANCED};
enum TargetRadio {ALSO, ONLY, SKIP};

#endif
