#include "MajSimai/MajSimai.h"

namespace MajSimai
{
    SimaiFileView::SimaiFileView(Core::UnmanagedSimaiParseResult result)
        : m_ptr(result.simaiFile), m_result(result)
    {
    }

    SimaiFileView::~SimaiFileView()
    {
        Core::MajSimai_Free(&m_result);
        m_result = {};
        m_ptr = nullptr;
    }

    SimaiFileView::SimaiFileView(SimaiFileView &&other) noexcept
        : m_ptr(other.m_ptr), m_result(other.m_result)
    {
        other.m_ptr = nullptr;
        other.m_result = {};
    }

    SimaiFileView &SimaiFileView::operator=(SimaiFileView &&other) noexcept
    {
        if (this != &other)
        {
            Core::MajSimai_Free(&m_result);

            m_ptr = other.m_ptr;
            m_result = other.m_result;

            other.m_ptr = nullptr;
            other.m_result = {};
        }

        return *this;
    }

    std::string_view SimaiFileView::GetTitle() const
    {
        return Core::MakeView(m_ptr->title, m_ptr->titleLen);
    }

    float SimaiFileView::GetOffset() const
    {
        return m_ptr->offset;
    }

    ArrayView<SimaiChartView, Core::UnmanagedSimaiChart>
    SimaiFileView::GetCharts() const
    {
        return {m_ptr->charts, static_cast<size_t>(m_ptr->chartsLen)};
    }
}