#include "MajSimai/MajSimai.h"

namespace MajSimai
{
    SimaiChartView::SimaiChartView(const Core::UnmanagedSimaiChart *ptr) : m_ptr(ptr) {}

    std::string_view SimaiChartView::GetLevel() const { return Core::MakeView(m_ptr->level, m_ptr->levelLen); }
    std::string_view SimaiChartView::GetDesigner() const { return Core::MakeView(m_ptr->designer, m_ptr->designerLen); }
    std::string_view SimaiChartView::GetFumen() const { return Core::MakeView(m_ptr->fumen, m_ptr->fumenLen); }

    ArrayView<SimaiTimingPointView, Core::UnmanagedSimaiTimingPoint> SimaiChartView::GetNoteTimings() const
    {
        return {m_ptr->noteTimings, static_cast<size_t>(m_ptr->noteTimingsLen)};
    }
}