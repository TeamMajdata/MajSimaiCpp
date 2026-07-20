#include "MajSimai/MajSimai.h"

namespace MajSimai
{
    SimaiNoteView::SimaiNoteView(const Core::UnmanagedSimaiNote *ptr) : m_ptr(ptr) {}

    SimaiNoteType SimaiNoteView::GetType() const { return m_ptr->type; }
    int32_t SimaiNoteView::GetStartPosition() const { return m_ptr->startPosition; }
    double SimaiNoteView::GetHoldTime() const { return m_ptr->holdTime; }
    bool SimaiNoteView::IsBreak() const { return m_ptr->isBreak; }

    std::string_view SimaiNoteView::GetRawContent() const
    {
        return Core::MakeView(m_ptr->rawContent, m_ptr->rawContentLen);
    }
}