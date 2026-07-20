#include "MajSimai/MajSimai.h"

namespace MajSimai
{
    SimaiTimingPointView::SimaiTimingPointView(const Core::UnmanagedSimaiTimingPoint *ptr) : m_ptr(ptr) {}

    double SimaiTimingPointView::GetTiming() const { return m_ptr->timing; }
    float SimaiTimingPointView::GetBpm() const { return m_ptr->bpm; }
    std::string_view SimaiTimingPointView::GetRawContent() const
    {
        return Core::MakeView(m_ptr->rawContent, m_ptr->rawContentLen);
    }

    ArrayView<SimaiNoteView, Core::UnmanagedSimaiNote> SimaiTimingPointView::GetNotes() const
    {
        return {m_ptr->notes, static_cast<size_t>(m_ptr->notesLen)};
    }
}