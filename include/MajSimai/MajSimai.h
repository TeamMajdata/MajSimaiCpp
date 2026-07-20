#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <string_view>

namespace MajSimai {
    enum class SimaiNoteType : int32_t
    {
        Tap = 0,
        Slide,
        Hold,
        Touch,
        TouchHold
    };
    namespace Core
    {
        extern "C"
        {
            struct UnmanagedSimaiNote {
                SimaiNoteType type;
                int32_t startPosition;
                double holdTime;
                double slideStartTime;
                double slideTime;
                char touchArea;
                bool isBreak, isEx, isFakeRotate, isForceStar, isHanabi, isSlideBreak, isSlideNoHead, isMine, isMineSlide, usingSV;
                int32_t rawContentLen;
                char* rawContent;
            };

            struct UnmanagedSimaiTimingPoint {
                double_t timing;
                float_t bpm, hSpeed, sVeloc;
                int32_t rawTextPositionX, rawTextPositionY, rawTextPosition, notesLen, rawContentLen;
                bool isEmpty;
                UnmanagedSimaiNote* notes;
                char* rawContent;
            };

            struct UnmanagedSimaiChart {
                int32_t levelLen, designerLen, fumenLen, noteTimingsLen, commaTimingsLen;
                bool isEmpty;
                char* level;
                char* designer;
                char* fumen;
                UnmanagedSimaiTimingPoint* noteTimings;
                UnmanagedSimaiTimingPoint* commaTimings;
            };

            struct UnmanagedSimaiCommand {
                int32_t prefixLen, valueLen;
                char* prefix;
                char* value;
            };

            struct UnmanagedSimaiFile {
                int32_t titleLen, artistLen, finalDesignerLen;
                float_t offset;
                int32_t chartsLen, commandsLen;
                char* title;
                char* artist;
                char* finalDesigner;
                UnmanagedSimaiChart* charts;
                UnmanagedSimaiCommand* commands;
            };

            struct UnmanagedSimaiParseResult {
                static constexpr int32_t CODE_NO_ERROR = 0;
                int32_t code, errorAtLine, errorAtColumn, errorMsgLen, errorContentLen;
                UnmanagedSimaiFile* simaiFile;
                char* errorMsgAnsi;
                char* errorContentAnsi;
            };

            void __cdecl MajSimai_Parse(const char* contentAnsi, int32_t contentAnsiLen, UnmanagedSimaiParseResult* result);
            bool __cdecl MajSimai_Free(UnmanagedSimaiParseResult* ptr);
        }

        inline std::string_view MakeView(const char* str, int32_t len) {
            if (!str || len <= 0) return {};
            return { str, static_cast<size_t>(len) };
        }

        inline std::string MakeString(const char* str, int32_t len) {
            if (!str || len <= 0) return std::string();
            return std::string(str, static_cast<size_t>(len));
        }
    }
    struct SimaiNote
    {
        SimaiNoteType type;
        int32_t startPosition;

        double holdTime;
        double slideStartTime;
        double slideTime;
        char touchArea;

        bool isBreak;
        bool isEx;
        bool isFakeRotate;
        bool isForceStar;
        bool isHanabi;
        bool isSlideBreak;
        bool isSlideNoHead;
        bool isMine;
        bool isMineSlide;
        bool usingSV;

        std::string rawContent;
    };

    struct SimaiTimingPoint {
        double timing;
        float bpm;
        float hSpeed;
        float sVeloc;
        int32_t rawTextPositionX;
        int32_t rawTextPositionY;
        int32_t rawTextPosition;
        bool isEmpty;

        std::vector<SimaiNote> notes;
        std::string rawContent;
    };

    struct SimaiChart {
        std::string level;
        std::string designer;
        std::string fumen;
        bool isEmpty;

        std::vector<SimaiTimingPoint> noteTimings;
        std::vector<SimaiTimingPoint> commaTimings;
    };

    struct SimaiCommand {
        std::string prefix;
        std::string value;
    };

    struct SimaiFile {
        std::string title;
        std::string artist;
        std::string finalDesigner;
        float offset;

        std::vector<SimaiChart> charts;
        std::vector<SimaiCommand> commands;
    };

    template <typename TView, typename TUnmanaged>
    class ArrayView
    {
        const TUnmanaged* m_data;
        size_t m_size;

    public:
        ArrayView(const TUnmanaged* data, size_t size);

        size_t Size() const;
        bool IsEmpty() const;

        TView operator[](size_t index) const;

        class Iterator
        {
            const TUnmanaged* ptr;

        public:
            explicit Iterator(const TUnmanaged* p);

            TView operator*() const;
            Iterator& operator++();
            bool operator!=(const Iterator& other) const;
        };

        Iterator begin() const;
        Iterator end() const;
    };

    class SimaiNoteView {
        const Core::UnmanagedSimaiNote* m_ptr;
    public:
        explicit SimaiNoteView(const Core::UnmanagedSimaiNote* ptr);

        SimaiNoteType GetType() const;
        int32_t GetStartPosition() const;
        double GetHoldTime() const;
        bool IsBreak() const;

        std::string_view GetRawContent() const;
    };

    class SimaiTimingPointView {
        const Core::UnmanagedSimaiTimingPoint* m_ptr;
    public:
        explicit SimaiTimingPointView(const Core::UnmanagedSimaiTimingPoint* ptr);

        double GetTiming() const;
        float GetBpm() const;
        std::string_view GetRawContent() const;

        ArrayView<SimaiNoteView, Core::UnmanagedSimaiNote> GetNotes() const;
    };

    class SimaiChartView {
        const Core::UnmanagedSimaiChart* m_ptr;
    public:
        explicit SimaiChartView(const Core::UnmanagedSimaiChart* ptr);

        std::string_view GetLevel() const;
        std::string_view GetDesigner() const;
        std::string_view GetFumen() const;

        ArrayView<SimaiTimingPointView, Core::UnmanagedSimaiTimingPoint> GetNoteTimings() const;
    };

    class SimaiFileView
    {
        const Core::UnmanagedSimaiFile* m_ptr{};
        Core::UnmanagedSimaiParseResult m_result{};
    public:
        explicit SimaiFileView(Core::UnmanagedSimaiParseResult result);
        ~SimaiFileView();

        SimaiFileView(const SimaiFileView&) = delete;
        SimaiFileView& operator=(const SimaiFileView&) = delete;

        SimaiFileView(SimaiFileView&& other) noexcept;
        SimaiFileView& operator=(SimaiFileView&& other) noexcept;

        std::string_view GetTitle() const;
        float GetOffset() const;

        ArrayView<SimaiChartView, Core::UnmanagedSimaiChart> GetCharts() const;
    };

    class SimaiParser
    {
    public:
        static SimaiFile Parse(std::string content);
        static SimaiFileView ParseAsView(std::string_view content);

    private:
        static SimaiFile ConvertFile(const Core::UnmanagedSimaiFile* uFile);
        static SimaiChart ConvertChart(const Core::UnmanagedSimaiChart& uChart);
        static SimaiTimingPoint ConvertTimingPoint(const Core::UnmanagedSimaiTimingPoint& uTp);
        static SimaiNote ConvertNote(const Core::UnmanagedSimaiNote& uNote);

        static void ThrowIfFailed(Core::UnmanagedSimaiParseResult* p_result);
    };
}
#include "ArrayView.inl"
