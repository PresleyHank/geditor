//
// Created by Alex Tsao(无间) on 2019/6/27.
//

#ifndef GEDITOR_LAYOUT_H
#define GEDITOR_LAYOUT_H

#include "common.h"
struct Offset {
    int x = 0;
    int y = 0;
    Offset() = default;
    Offset(int x, int y) : x(x), y(y) {}
    inline Offset operator+(const Offset &offset) {
        return {x + offset.x, y + offset.y};
    }
    inline Offset operator-(const Offset &offset) {
        return {x - offset.x, y - offset.y};
    }
    inline Offset &operator+=(const Offset &offset) {
        x += offset.x;
        y += offset.y;
        return *this;
    }
    inline Offset &operator-=(const Offset &offset) {
        x -= offset.x;
        y -= offset.y;
        return *this;
    }
    inline bool operator==(const Offset &offset) { return x == offset.x && y == offset.y; }
    inline bool operator!=(const Offset &offset) { return !(x == offset.x && y == offset.y); }

};

//typedef int Display;
enum Display {
    DisplayNone,
    DisplayAbsolute,
    DisplayInline,
    DisplayBlock,
    DisplayTable,
    DisplayRow,
    DisplayCustom,
};
class Root;
class RelativeElement;
class Document;
class Element;
class LayoutManager;
class RenderManager;
struct EventContext;
struct LayoutContext {
    int lineMaxHeight = 0;
    int blockMaxWidth = 0;
};
#define LayoutArgs() EventContext &context, LayoutManager *sender, Display display, LayoutContext &layoutContext, Offset &self, Offset &next
#define UseDisplayFunc(Fun) Fun(context, sender, display, layoutContext, self, next)
#define CallDisplayFunc(Fun) Fun(context, sender, display, layoutContext, self, next)
#define Layout(display) void display (LayoutArgs())
typedef void (*LayoutFunc) (LayoutArgs());

Layout(LayoutDisplayNone);
Layout(LayoutDisplayAbsolute);
Layout(LayoutDisplayInline);
Layout(LayoutDisplayBlock);
Layout(LayoutDisplayTable);
Layout(LayoutDisplayRow);
Layout(LayoutDisplayCustom);

class LayoutManager {
private:
    RenderManager *m_renderManager;
public:
    explicit LayoutManager(RenderManager *renderManager);
    LayoutFunc m_layouts[7] = {
            LayoutDisplayNone,
            LayoutDisplayAbsolute,
            LayoutDisplayInline,
            LayoutDisplayBlock,
            LayoutDisplayTable,
            LayoutDisplayRow,
            LayoutDisplayCustom,
    };
    void reflow(EventContext context, bool relayout = false, Offset offset = {0, 0});
    LayoutFunc func(Display display) { return m_layouts[display]; }
};

#endif //GEDITOR_LAYOUT_H
