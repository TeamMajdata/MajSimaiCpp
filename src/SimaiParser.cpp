#include "MajSimai/MajSimai.h"

namespace MajSimai
{
    SimaiFile SimaiParser::Parse(std::string content)
    {
        Core::UnmanagedSimaiParseResult result{};
        auto ptr = content.c_str();
        Core::MajSimai_Parse(content.c_str(), content.length(), &result);

        ThrowIfFailed(&result);

        return ConvertFile(result.simaiFile);
    }

    SimaiFileView SimaiParser::ParseAsView(std::string_view content)
    {
        Core::UnmanagedSimaiParseResult result{};

        Core::MajSimai_Parse(content.data(), static_cast<int32_t>(content.length()), &result);

        ThrowIfFailed(&result);

        return SimaiFileView(result);
    }

    SimaiFile SimaiParser::ConvertFile(const Core::UnmanagedSimaiFile *uFile)
    {
        SimaiFile file;
        if (!uFile)
            return file;

        file.title = Core::MakeString(uFile->title, uFile->titleLen);
        file.artist = Core::MakeString(uFile->artist, uFile->artistLen);
        file.finalDesigner = Core::MakeString(uFile->finalDesigner, uFile->finalDesignerLen);
        file.offset = uFile->offset;

        if (uFile->charts && uFile->chartsLen > 0)
        {
            file.charts.reserve(uFile->chartsLen);
            for (int32_t i = 0; i < uFile->chartsLen; ++i)
            {
                file.charts.push_back(ConvertChart(uFile->charts[i]));
            }
        }

        if (uFile->commands && uFile->commandsLen > 0)
        {
            file.commands.reserve(uFile->commandsLen);
            for (int32_t i = 0; i < uFile->commandsLen; ++i)
            {
                SimaiCommand cmd;
                cmd.prefix = Core::MakeString(uFile->commands[i].prefix, uFile->commands[i].prefixLen);
                cmd.value = Core::MakeString(uFile->commands[i].value, uFile->commands[i].valueLen);
                file.commands.push_back(std::move(cmd));
            }
        }

        return file;
    }

    SimaiChart SimaiParser::ConvertChart(const Core::UnmanagedSimaiChart &uChart)
    {
        SimaiChart chart;
        chart.level = Core::MakeString(uChart.level, uChart.levelLen);
        chart.designer = Core::MakeString(uChart.designer, uChart.designerLen);
        chart.fumen = Core::MakeString(uChart.fumen, uChart.fumenLen);
        chart.isEmpty = uChart.isEmpty;

        auto convertTimings = [](const Core::UnmanagedSimaiTimingPoint *uTimings, int32_t len)
        {
            std::vector<SimaiTimingPoint> timings;
            if (uTimings && len > 0)
            {
                timings.reserve(len);
                for (int32_t i = 0; i < len; ++i)
                {
                    timings.push_back(ConvertTimingPoint(uTimings[i]));
                }
            }
            return timings;
        };

        chart.noteTimings = convertTimings(uChart.noteTimings, uChart.noteTimingsLen);
        chart.commaTimings = convertTimings(uChart.commaTimings, uChart.commaTimingsLen);

        return chart;
    }

    SimaiTimingPoint SimaiParser::ConvertTimingPoint(const Core::UnmanagedSimaiTimingPoint &uTp)
    {
        SimaiTimingPoint tp;
        tp.timing = uTp.timing;
        tp.bpm = uTp.bpm;
        tp.hSpeed = uTp.hSpeed;
        tp.sVeloc = uTp.sVeloc;
        tp.rawTextPositionX = uTp.rawTextPositionX;
        tp.rawTextPositionY = uTp.rawTextPositionY;
        tp.rawTextPosition = uTp.rawTextPosition;
        tp.isEmpty = uTp.isEmpty;
        tp.rawContent = Core::MakeString(uTp.rawContent, uTp.rawContentLen);

        if (uTp.notes && uTp.notesLen > 0)
        {
            tp.notes.reserve(uTp.notesLen);
            for (int32_t i = 0; i < uTp.notesLen; ++i)
            {
                tp.notes.push_back(ConvertNote(uTp.notes[i]));
            }
        }

        return tp;
    }

    SimaiNote SimaiParser::ConvertNote(const Core::UnmanagedSimaiNote &uNote)
    {
        SimaiNote note;
        note.type = uNote.type;
        note.startPosition = uNote.startPosition;
        note.holdTime = uNote.holdTime;
        note.slideStartTime = uNote.slideStartTime;
        note.slideTime = uNote.slideTime;
        note.touchArea = uNote.touchArea;
        note.isBreak = uNote.isBreak;
        note.isEx = uNote.isEx;
        note.isFakeRotate = uNote.isFakeRotate;
        note.isForceStar = uNote.isForceStar;
        note.isHanabi = uNote.isHanabi;
        note.isSlideBreak = uNote.isSlideBreak;
        note.isSlideNoHead = uNote.isSlideNoHead;
        note.isMine = uNote.isMine;
        note.isMineSlide = uNote.isMineSlide;
        note.usingSV = uNote.usingSV;
        note.rawContent = Core::MakeString(uNote.rawContent, uNote.rawContentLen);

        return note;
    }

    void SimaiParser::ThrowIfFailed(Core::UnmanagedSimaiParseResult *p_result)
    {
        if (p_result->code != Core::UnmanagedSimaiParseResult::CODE_NO_ERROR)
        {
            std::string errorMsg = "Unknown error";
            if (p_result->errorMsgAnsi)
            {
                errorMsg.assign(p_result->errorMsgAnsi, p_result->errorMsgLen);
            }
            Core::MajSimai_Free(p_result);
            *p_result = {};
            throw std::runtime_error("Simai parsing failed: " + errorMsg);
        }
    }
}