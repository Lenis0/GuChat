#ifndef GLOBAL_H
#define GLOBAL_H
#include <QStyle>
#include <QWidget>
#include <functional>

extern std::function<void(QWidget *)> repolish; // 重刷新qss

#endif // GLOBAL_H
