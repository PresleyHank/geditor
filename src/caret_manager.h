//
// Created by Administrator on 2019/9/3.
//

#ifndef GEDITOR_CARET_MANAGER_H
#define GEDITOR_CARET_MANAGER_H

#include "paint_manager.h"

struct CaretData {
    int index = 0;
    void *data = nullptr;
    CaretData() = default;
    CaretData(int index, void *data) : index(index), data(data) {}
};

class CaretManager {
    friend PaintManager;
private:
    PaintManager *m_paintManager;
    EventContext *m_context = nullptr;
    Offset m_current;
    CaretData m_data;
public:
    explicit CaretManager(PaintManager *paintManager) : m_paintManager(paintManager) {}
    ~CaretManager();
    Element *getFocus ();
    EventContext *getEventContext() { return m_context; }
    void create() {
        CreateCaret(m_paintManager->m_hWnd, nullptr, 1, 15);
    }
    inline CaretData *data() {
        return &m_data;
    }
    void focus(EventContext *context);
    void show() {
        ShowCaret(m_paintManager->m_hWnd);
    }
    void hide() {
        SetCaretPos(-1, -1);
    }
    void set(Offset pos) {
        m_current = pos;
        update();
    }
    // 设置相对的光标位置
    void set(int x, int y) {
        m_current.x = x;
        m_current.y = y;
        update();
    }

    void autoSet(int x, int y);
    Offset get() { return m_current; }
    void update();

};


#endif //GEDITOR_CARET_MANAGER_H