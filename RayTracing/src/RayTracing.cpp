#include "Renderer.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
using namespace Walnut;

class ExampleLayer : public Walnut::Layer {
 public:
  virtual void OnUIRender() override {
    ImGui::Begin("Settings");
    ImGui::Text("Last Render Time: %.3fms", m_LastRenderTimer);
    if (ImGui::Button("Render")) {
      Render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ViewPort");
    m_viewportHeight = ImGui::GetContentRegionAvail().y;
    m_viewportWidth = ImGui::GetContentRegionAvail().x;

    auto image = m_Renderer.GetFinalImage();
    if (image)
      ImGui::Image(image->GetDescriptorSet(),
                   {(float)image->GetWidth(), (float)image->GetHeight()}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
    Render();
  }
  void Render() {
    Timer timer;

    m_Renderer.OnResize(m_viewportWidth, m_viewportHeight);
    m_Renderer.Render();

    m_LastRenderTimer = timer.ElapsedMillis();
  }

 private:
  Renderer m_Renderer;
  uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
  float m_LastRenderTimer = 0.0f;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
  Walnut::ApplicationSpecification spec;
  spec.Name = "Ray Tracing";

  Walnut::Application *app = new Walnut::Application(spec);
  app->PushLayer<ExampleLayer>();
  app->SetMenubarCallback([app]() {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        app->Close();
      }
      ImGui::EndMenu();
    }
  });
  return app;
}