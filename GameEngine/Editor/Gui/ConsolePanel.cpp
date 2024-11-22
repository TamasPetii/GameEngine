#include "ConsolePanel.h"

void ConsolePanel::Update()
{
}

void ConsolePanel::Render()
{
	if (GlobalSettings::GameViewActive)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Console"))
	{
        if (ImGui::BeginTable("Console##ConsolePanel", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
        {
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Origin", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (auto& logEntry : Logger::Instance()->GetEntries())
            {
                    ImVec4 color = ImVec4(1, 1, 1, 1);

                    if(logEntry.type == LogType::LOG_ERROR)
                        color = ImVec4(1, 0, 0, 1);
                    else if(logEntry.type == LogType::LOG_INFO)
                        color = ImVec4(0, 0.5, 1, 1);

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(color, logEntry.timestamp.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextColored(color, logEntry.origin.c_str());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextColored(color, logEntry.message.c_str());
            }

            ImGui::EndTable();
        }
	}

	ImGui::End();
	ImGui::PopStyleVar();
}