#include "core.hpp"
#include "gl-debug.hpp"
#include <random>
#include <unordered_set>
#include <format>
#include <chrono>

#include "gl-frame-dump.hpp"

/*
struct ubo_struct{
	glm::mat4 mvp;       //64[16]
}ubo_data;
*/


class prepare{
	public:
		void set_up_current_ctx(core::sdl_event_ctx &ctx);
		void set_up_gl_capabilities(core::sdl_event_ctx &ctx);
		void set_up_sdl_audio(core::sdl_event_ctx &ctx);
		void set_up_gl_objects(core::sdl_event_ctx &ctx);
		//void set_up_gl_program(core::sdl_event_ctx &ctx);

	public:
		void all(core::sdl_event_ctx &ctx){
			set_up_current_ctx(ctx);
			set_up_sdl_audio(ctx);

			//set_up_gl_capabilities(ctx);
			set_up_gl_objects(ctx);
			//set_up_gl_program(ctx);
		}
};

class frame{
	public:
		void imgui_window(core::sdl_event_ctx &ctx);

	public:
		void handle_events(core::sdl_event_ctx &ctx);
		void init_frame(core::sdl_event_ctx &ctx);
		void draw_scene(core::sdl_event_ctx &ctx);
		void build_ui(core::sdl_event_ctx &ctx);
		void draw_ui(core::sdl_event_ctx &ctx);
		void render_frame(core::sdl_event_ctx &ctx);

	//all
	public:
		void one_frame(core::sdl_event_ctx &ctx){
			handle_events(ctx);
			init_frame(ctx);

			build_ui(ctx);

			draw_scene(ctx);
			draw_ui(ctx);

			render_frame(ctx);
		}
};

//forward decl
void colorful_style();
void profile_info(core::sdl_event_ctx &ctx);
void page_profile_alpha(core::sdl_event_ctx &ctx);
void page_profile_beta(core::sdl_event_ctx &ctx);

bool hovered_once(){
	static std::unordered_set<ImGuiID> hovered;

	ImGuiID id = ImGui::GetItemID();
	bool isHovered = ImGui::IsItemHovered();

	if (isHovered && hovered.find(id) == hovered.end()){
		hovered.insert(id);
		return true;
	}

	if (!isHovered){
		hovered.erase(id);
	}

	return false;
}

void draw_count_card(
		int64_t count,
		std::string title = "Colorful counter",
		std::string num_suffix = "",
		std::string label = "current times",
		ImVec2 size = {300,300},
		float rounding=12.f,
		float title_font_size = 20.f,
		float num_font_size = 56.f,
		float label_font_size = 16.f
		);

void prepare::set_up_gl_capabilities(core::sdl_event_ctx &ctx){

	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	//glEnable(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(std::numeric_limits<GLuint>::max());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
}


void prepare::set_up_gl_objects(core::sdl_event_ctx &ctx){
	auto &raw_avatar_alpha_png = romfs::get("icon/avatar_alpha.png");
	auto &raw_avatar_beta_png = romfs::get("icon/avatar_beta.png");

	ctx.avatar_alpha.png_io_image2d_with_target(
		raw_avatar_alpha_png.data(),
		raw_avatar_alpha_png.size()
	);

	ctx.avatar_beta.png_io_image2d_with_target(
		raw_avatar_beta_png.data(),
		raw_avatar_beta_png.size()
	);

	if (!ctx.avatar_alpha.is_ok()){
		ctx.status = false;
		ctx.reason = ctx.avatar_alpha.what();
		return;
	}

	if (!ctx.avatar_beta.is_ok()){
		ctx.status = false;
		ctx.reason = ctx.avatar_beta.what();
		return;
	}

	std::string romfs_path{};
	for(int i=0; i < std::size(ctx.avatar_extra); i++){
		romfs_path = std::format("icon/extra{:d}.png", i);
		auto raw_extra_png = romfs::get(romfs_path.c_str());
		ctx.avatar_extra[i].png_io_image2d_with_target(raw_extra_png.data(), raw_extra_png.size());
		if (!ctx.avatar_extra[i].is_ok()){
			ctx.status = false;
			ctx.reason = ctx.avatar_beta.what();
			return;
		}
	}

}


void prepare::set_up_current_ctx(core::sdl_event_ctx &ctx){
	if(!SDL_GL_MakeCurrent(ctx.swm.window,ctx.sgcm.gl_ctx)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_MakeCurrent";
		return;
	}

	if(!SDL_GL_SetSwapInterval(1)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_SetSwapInterval";
		return;
	}

	ctx.status = true;
	ctx.running = true;
}

void prepare::set_up_sdl_audio(core::sdl_event_ctx &ctx){

	//bind
	if(ctx.realtime_audio.is_ok()){
		ctx.realtime_audio.bind();
	}else{
		ctx.status = false;
		ctx.reason = ctx.realtime_audio.what();
	}

	auto &raw_click_wav = romfs::get("wav/click.wav");
	auto &raw_hover_wav = romfs::get("wav/hover.wav");

	core::load_wav_from_mem_and_convert(
		raw_click_wav.data(), raw_click_wav.size(),
		ctx.realtime_audio.get_dst_spec(), ctx.click_wav
	);

	core::load_wav_from_mem_and_convert(
		raw_hover_wav.data(), raw_hover_wav.size(),
		ctx.realtime_audio.get_dst_spec(), ctx.hover_wav
	);

}


void frame::handle_events(core::sdl_event_ctx &ctx){

	while(SDL_PollEvent(&ctx.e)){
		if (ctx.e.type == SDL_EVENT_QUIT){ctx.running = false;}

		do{//change running state in main window
			if (ctx.e.window.windowID != SDL_GetWindowID(ctx.swm.window))break;

			if (ctx.e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED){ctx.running = false;}
			if (ctx.e.type == SDL_EVENT_KEY_DOWN && ctx.e.key.key == SDLK_ESCAPE){ctx.running = false;}
		}while(false);

		ImGui_ImplSDL3_ProcessEvent(&ctx.e);
	}

	if (ctx.window_draggable && ctx.keyboard.is_pressed(SDL_SCANCODE_SPACE)){
		ctx.window_draggable = false;
	}

}


void frame::draw_scene(core::sdl_event_ctx &ctx){
	//debug
	core::debug_error();
}

void frame::imgui_window(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");
	IMGUI_CHECKVERSION();

	ImGui::NewFrame();

	//style
	static bool once = true;
	if(once){
		colorful_style();
		once = false;
	}

	//drawlist and tips
	#if 1
	ImGuiViewport *viewport = ImGui::GetMainViewport();

	if(ctx.window_draggable){
		ImGuiWindowFlags tips_window_flags{};
		tips_window_flags |= ImGuiWindowFlags_NoMove;
		tips_window_flags |= ImGuiWindowFlags_NoResize;
		tips_window_flags |= ImGuiWindowFlags_NoDecoration;
		tips_window_flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::SetNextWindowPos((*viewport).GetCenter(),0,ImVec2{0.5,0.5});
		if(ImGui::Begin("Tips",nullptr,tips_window_flags)){
			ImGui::Text("PRESS SPACE TO STOP");
		}
		ImGui::End();
	}
	#endif

	#if 1

	ImGui::SetNextWindowPos((*viewport).Pos);
	ImGui::SetNextWindowSize((*viewport).Size);

	ImGuiWindowFlags main_window_flags{};
	main_window_flags |= ImGuiWindowFlags_NoMove;
	main_window_flags |= ImGuiWindowFlags_NoResize;
	main_window_flags |= ImGuiWindowFlags_NoDecoration;
	main_window_flags |= ImGuiWindowFlags_NoTitleBar;


	if(ImGui::Begin("##main window",nullptr,main_window_flags)){
		
		ImVec2 cra = ImGui::GetContentRegionAvail();

		static bool enable_left_side = true;
		ImVec2 left_size;
		ImVec2 right_size;
		ImVec2 bottom_size;

		if(enable_left_side){
			left_size = ImVec2{cra.x * ctx.leftw_pct, cra.y * (1.f - ctx.bottomh_pct)};
			right_size = ImVec2{cra.x * (1.f - ctx.leftw_pct), cra.y * (1.f - ctx.bottomh_pct)};
			bottom_size = ImVec2{cra.x, cra.y * ctx.bottomh_pct};
		}else{
			left_size = ImVec2{0,0};
			right_size = ImVec2{cra.x,  cra.y * (1.f - ctx.bottomh_pct)};
			bottom_size = ImVec2{cra.x, cra.y * ctx.bottomh_pct};
		}

		if (ctx.hide_sidebar){
			left_size = ImVec2{0,0};
			right_size = ImVec2{cra.x,  cra.y};
			bottom_size = ImVec2{0,0};
		}

		//#########
		//left side
		//#########
		if (enable_left_side && !ctx.hide_sidebar){
			if(ImGui::BeginChild("left",left_size,ImGuiChildFlags_Borders)){
				cra = ImGui::GetContentRegionAvail();
				ImVec2 button_size = ImVec2{cra.x , cra.y/20.f};

				//drag window
				if(ImGui::Button("Drag window",button_size)){
					ctx.window_draggable = true;

					ctx.click();
				}

				if(hovered_once()){ctx.hover();}


				//add button func
				auto add_button = [&ctx,&button_size](const char *name, core::page_status page){
					bool pop = false;
					ImU32 color_selected = IM_COL32(0xff,0xa5,0x00,0xa5);
					if(ctx.page == page){
						ImGui::PushStyleColor(ImGuiCol_Button, color_selected);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_selected);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_selected);
						pop = true;
					}

					if(ImGui::Button(name,button_size)){
						ctx.page = page;

						ctx.click();
					}

					if(hovered_once()){ctx.hover();}

					if(pop){
						ImGui::PopStyleColor(3);
						pop = false;
					}
				};

				add_button("Profile(alpha)", core::PAGE_PROFILE_ALPHA);
				add_button("Profile(beta)", core::PAGE_PROFILE_BETA);

			}
			ImGui::EndChild();

		}

		ImGui::SameLine(0.f,0.f);

		//##########
		//right side
		//##########
		if(ImGui::BeginChild("right",right_size,ImGuiChildFlags_Borders)){

			switch(ctx.page){
				case core::PAGE_PROFILE_ALPHA:{
					page_profile_alpha(ctx);
					break;
				}

				case core::PAGE_PROFILE_BETA:{
					page_profile_beta(ctx);
					break;
				}
			}

		}
		ImGui::EndChild();

		//#################
		//bottom status bar
		//#################
		if (!ctx.hide_sidebar){
			if(ImGui::BeginChild("bottom",bottom_size,ImGuiChildFlags_Borders)){

				ImGuiDir button_dir;
				if (enable_left_side == true)
					button_dir = ImGuiDir_Right;
				else{
					button_dir = ImGuiDir_Left;
				}

				if(ImGui::ArrowButton("left control", button_dir)){
					enable_left_side ^= true;
					ctx.click();
				}
				if(hovered_once()){ctx.hover();}

				ImGui::SameLine();

				const std::vector<std::string> texts = {
					"无疑，读到这段文字的你，很可能是幸福的",
					"结构决定功能",
					"环境的产物",
					"无呼吸义务",
					"幸福快乐没一天",
					"花言巧语，糖衣炮弹，望梅止渴，画饼充饥，落井下石",
					"希望寄托出去，期望奇迹发生(抽象实现抛弃你，期望落空弄死你)",
					"极低空间复杂度的资本，面对极不可控大数量变量，要求能跑且极低时间复杂度？",
				};

				static size_t index = 0;
				static std::mt19937 gen(std::random_device{}());

				double now = ImGui::GetTime();
				static double last = 0.0;

				if (now - last >= 5.0 && !texts.empty()){
					last = now;
					std::uniform_int_distribution<size_t> dist(0, texts.size() - 1);
					index = dist(gen);
				}

				ImGui::Text("%s", texts[index].c_str());
			}
			ImGui::EndChild();
		}

	}

	ImGui::End();
	#endif

	ImGui::EndFrame();
}


void frame::build_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	imgui_window(ctx);
}

void frame::draw_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(ctx.swm.window, ctx.sgcm.gl_ctx);
	}
}

void frame::init_frame(core::sdl_event_ctx &ctx){

	//clear
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(0.f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	#if 0
	int w,h;
	SDL_GetWindowSize(ctx.swm.window,&w,&h);
	ctx.projection = glm::perspective(glm::radians(45.f),(float)w/h,100.f,0.1f);
	#endif
}

void frame::render_frame(core::sdl_event_ctx &ctx){

	if (!core::is_offscreen){
		if(!SDL_GL_SwapWindow(ctx.swm.window)){
			ctx.running = false;
			ctx.status = false;
			ctx.reason = "[frame::render_frame]SDL_GL_SwapWindow";
			return;
		}

		//SDL_Delay(0);
		//ctx.running = false;
	}else{
		glFinish();

		int w,h;
		SDL_GetWindowSize(ctx.swm.window,&w,&h);
		core::frame_dump_png(w,h, ctx.png_path.c_str());

		ctx.running = false;
	}

}

void draw_count_card(
		int64_t count,
		std::string title,
		std::string num_suffix,
		std::string label,
		ImVec2 size,
		float rounding,
		float title_font_size,
		float num_font_size,
		float label_font_size
		){


	auto anime_color = [](ImU32 startColor, float speed = 0.08f) -> ImU32 {
		ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(startColor);

		float h{}, s{}, v{};
		ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, h, s, v);

		float time = ImGui::GetTime() * speed;
		h = fmodf(h + time, 1.0f);

		ImVec4 ret_rgba;
		ImGui::ColorConvertHSVtoRGB(h, s, v, ret_rgba.x, ret_rgba.y, ret_rgba.z);
		ret_rgba.w = rgba.w;

		return ImGui::ColorConvertFloat4ToU32(ret_rgba);
	};

	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 card_pos = ImGui::GetCursorScreenPos();
	//ImVec2 card_size = ImGui::GetContentRegionAvail();
	ImVec2 card_size = size;

	ImU32 col_bg = anime_color(IM_COL32(0x39, 0xc5, 0xbb, 0xff));
	ImU32 col_tx = anime_color(IM_COL32(0xf5, 0xf5, 0xf5, 0xf5));

	//bg and shadow and border
	ImVec2 bg_pos_max = ImVec2(card_pos.x + card_size.x, card_pos.y + card_size.y);
	draw->AddRectFilled(card_pos, bg_pos_max , col_bg, rounding);
	draw->AddRect(card_pos, bg_pos_max , col_tx, rounding);
	draw->AddRectFilled(card_pos, bg_pos_max , IM_COL32(0, 0, 0, 0x32), rounding);

	ImVec2 title_pos = ImVec2(card_pos.x + 0.1*card_size.x, card_pos.y + 0.1*card_size.y);
	(*draw).AddText(ImGui::GetFont(), title_font_size, title_pos, col_tx, title.c_str());

	std::string buf = std::to_string(count);
	buf += num_suffix;
	ImVec2 num_pos = ImVec2(card_pos.x  + 0.2*card_size.x, card_pos.y + 0.5*card_size.y);
	(*draw).AddText(ImGui::GetFont(), num_font_size, num_pos, col_tx , buf.c_str());

	ImVec2 line_pos_min = ImVec2(card_pos.x  + 0.2*card_size.x, card_pos.y + 0.8*card_size.y);
	ImVec2 line_pos_max = ImVec2(line_pos_min.x + 0.7 *card_size.x,line_pos_min.y + 0.01*card_size.y);
	(*draw).AddRect(line_pos_min, line_pos_max, col_tx, rounding);

	ImVec2 label_pos = ImVec2(card_pos.x  + 0.25*card_size.x, card_pos.y + 0.86*card_size.y);
	(*draw).AddText(ImGui::GetFont(), label_font_size, label_pos, col_tx, label.c_str());

	ImGui::Dummy(size);
}

void colorful_style(){
	ImGuiStyle& style = ImGui::GetStyle();

	// ---------- Font scaling ----------
	style.FontSizeBase = 18.0000f;
	style.FontScaleMain = 1.0000f;
	style.FontScaleDpi = 1.0000f;

	// ---------- Alpha ----------
	style.Alpha = 1.0000f;
	style.DisabledAlpha = 0.5000f;

	// ---------- Window ----------
	style.WindowPadding = ImVec2(10.0000f, 10.0000f);
	style.WindowRounding = 6.0000f;
	style.WindowBorderSize = 1.0000f;
	style.WindowBorderHoverPadding = 4.0000f;
	style.WindowMinSize = ImVec2(32.0000f, 32.0000f);
	style.WindowTitleAlign = ImVec2(0.0000f, 0.5000f);
	style.WindowMenuButtonPosition = (ImGuiDir)0;

	// ---------- Child & Popup ----------
	style.ChildRounding = 6.0000f;
	style.ChildBorderSize = 1.0000f;
	style.PopupRounding = 6.0000f;
	style.PopupBorderSize = 1.0000f;

	// ---------- Frame & Item ----------
	style.FramePadding = ImVec2(5.0000f, 3.0000f);
	style.FrameRounding = 6.0000f;
	style.FrameBorderSize = 0.0000f;
	style.ItemSpacing = ImVec2(8.0000f, 5.0000f);
	style.ItemInnerSpacing = ImVec2(5.0000f, 4.0000f);
	style.CellPadding = ImVec2(5.0000f, 2.0000f);
	style.TouchExtraPadding = ImVec2(0.0000f, 0.0000f);
	style.IndentSpacing = 21.0000f;
	style.ColumnsMinSpacing = 6.0000f;

	// ---------- Scrollbar ----------
	style.ScrollbarSize = 12.0000f;
	style.ScrollbarRounding = 6.0000f;
	style.ScrollbarPadding = 2.0000f;

	// ---------- Grab ----------
	style.GrabMinSize = 12.0000f;
	style.GrabRounding = 6.0000f;
	style.LogSliderDeadzone = 4.0000f;

	// ---------- Image ----------
	style.ImageRounding = 6.0000f;
	style.ImageBorderSize = 0.0000f;

	// ---------- Tab ----------
	style.TabRounding = 6.0000f;
	style.TabBorderSize = 0.0000f;
	style.TabMinWidthBase = 1.0000f;
	style.TabMinWidthShrink = 80.0000f;
	style.TabCloseButtonMinWidthSelected = -1.0000f;
	style.TabCloseButtonMinWidthUnselected = 0.0000f;
	style.TabBarBorderSize = 1.0000f;
	style.TabBarOverlineSize = 2.0000f;

	// ---------- Table ----------
	style.TableAngledHeadersAngle = 0.6109f;
	style.TableAngledHeadersTextAlign = ImVec2(0.5000f, 0.0000f);

	// ---------- TreeLines ----------
	style.TreeLinesFlags = (ImGuiTreeNodeFlags)262144;
	style.TreeLinesSize = 1.0000f;
	style.TreeLinesRounding = 6.0000f;

	// ---------- MenuItem & Selectable ----------
	style.MenuItemRounding = 6.0000f;
	style.SelectableRounding = 6.0000f;
	style.DragDropTargetRounding = 6.0000f;
	style.DragDropTargetBorderSize = 2.0000f;
	style.DragDropTargetPadding = 3.0000f;
	style.ColorMarkerSize = 3.0000f;
	style.ColorButtonPosition = (ImGuiDir)1;
	style.ButtonTextAlign = ImVec2(0.5000f, 0.5000f);
	style.SelectableTextAlign = ImVec2(0.0000f, 0.0000f);
	style.InputTextCursorSize = 1.0000f;

	// ---------- Separator ----------
	style.SeparatorSize = 1.0000f;
	style.SeparatorTextBorderSize = 2.0000f;
	style.SeparatorTextAlign = ImVec2(0.0000f, 0.5000f);
	style.SeparatorTextPadding = ImVec2(20.0000f, 3.0000f);

	// ---------- Display ----------
	style.DisplayWindowPadding = ImVec2(19.0000f, 19.0000f);
	style.DisplaySafeAreaPadding = ImVec2(3.0000f, 3.0000f);

	// ---------- Docking ----------
	style.DockingNodeHasCloseButton = true;
	style.DockingSeparatorSize = 2.0000f;

	// ---------- Mouse Cursor ----------
	style.MouseCursorScale = 1.0000f;

	// ---------- Anti-Aliasing ----------
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill = true;

	// ---------- Tessellation ----------
	style.CurveTessellationTol = 1.2500f;
	style.CircleTessellationMaxError = 0.3000f;

	// ---------- Colors ----------
	style.Colors[ImGuiCol_Text] = ImVec4(0.0800f, 0.1500f, 0.1800f, 1.0000f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4500f, 0.5200f, 0.5500f, 1.0000f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.8510f, 0.9098f, 0.9216f, 0.8667f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.8800f, 0.9300f, 0.9400f, 0.5000f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.9000f, 0.9400f, 0.9500f, 0.9800f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.5000f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7200f, 0.8100f, 0.8300f, 1.0000f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.7000f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.9000f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.7800f, 0.8600f, 0.8700f, 0.9500f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.9500f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.7800f, 0.8600f, 0.8700f, 0.6000f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8200f, 0.8900f, 0.9000f, 0.9500f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.7800f, 0.8600f, 0.8700f, 0.6000f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.7000f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1800f, 0.6800f, 0.6400f, 0.9000f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0000f, 0.6471f, 0.0000f, 1.0000f);
	style.Colors[ImGuiCol_CheckboxSelectedBg] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8000f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2240f, 0.7730f, 0.7330f, 1.0000f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2240f, 0.7730f, 0.7330f, 1.0000f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1800f, 0.6800f, 0.6400f, 1.0000f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.6000f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8500f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 0.9000f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.4500f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8000f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.4000f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8000f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_InputTextCursor] = ImVec4(0.2240f, 0.7730f, 0.7330f, 1.0000f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8500f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.7800f, 0.8600f, 0.8700f, 0.9000f);
	style.Colors[ImGuiCol_TabSelected] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.9500f);
	style.Colors[ImGuiCol_TabSelectedOverline] = ImVec4(1.0000f, 0.6800f, 0.8200f, 0.9000f);
	style.Colors[ImGuiCol_TabDimmed] = ImVec4(0.8000f, 0.8700f, 0.8800f, 0.8000f);
	style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.1800f, 0.6000f, 0.5700f, 0.9000f);
	style.Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(1.0000f, 0.6800f, 0.8200f, 0.0000f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.5000f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.8500f, 0.9100f, 0.9200f, 1.0000f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.2240f, 0.7730f, 0.7330f, 1.0000f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.2240f, 0.7730f, 0.7330f, 1.0000f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7800f, 0.8600f, 0.8700f, 1.0000f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.8000f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.3000f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0000f, 0.0000f, 0.0000f, 0.0000f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.0800f);
	style.Colors[ImGuiCol_TextLink] = ImVec4(0.8500f, 0.4000f, 0.6000f, 1.0000f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0000f, 0.6800f, 0.8200f, 0.4000f);
	style.Colors[ImGuiCol_TreeLines] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.5000f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.2500f);
	style.Colors[ImGuiCol_UnsavedMarker] = ImVec4(1.0000f, 0.6800f, 0.8200f, 1.0000f);
	style.Colors[ImGuiCol_NavCursor] = ImVec4(0.2240f, 0.7730f, 0.7330f, 0.9000f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0000f, 0.6800f, 0.8200f, 0.8000f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8500f, 0.9100f, 0.9200f, 0.4000f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8500f, 0.9100f, 0.9200f, 0.5000f);

	// ---------- Hover Behaviors ----------
	style.HoverStationaryDelay = 0.1500f;
	style.HoverDelayShort = 0.1500f;
	style.HoverDelayNormal = 0.4000f;
	style.HoverFlagsForTooltipMouse = (ImGuiHoveredFlags)41984;
	style.HoverFlagsForTooltipNav = (ImGuiHoveredFlags)197632;
}

void profile_info(core::sdl_event_ctx &ctx, ImVec2 size, ImVec4 hl_col_positive, ImVec4 hl_col_negative){
	if(ImGui::BeginChild("profile text right", size)){

		ImGui::Text("ID: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "condexpr01");

		ImGui::Text("Preferred Name: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "Vito Devlin");

		ImGui::Text("Birthday: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "2005-12-09");

		ImGui::Text("Mail: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "condexpr01@{outlook.com, gmail.com}");

		ImGui::Text("Status: ");
		ImGui::SameLine();

		auto dead_tp = std::chrono::sys_days{std::chrono::year_month_day{
			std::chrono::year(2026),
				std::chrono::month(11),
				std::chrono::day(11),
			}}
			+ std::chrono::hours{-8}//CST
			+ std::chrono::minutes{0}
			+ std::chrono::seconds{0}
		;

		bool is_alive = false;
		if (std::chrono::system_clock::now() >= dead_tp){
			is_alive = false;
		}else{
			is_alive = true;
		}


		if (is_alive){
			ImGui::TextColored(hl_col_positive, "Alive(Happy Happy Happy)");
		}else{
			//UTC TIMEPOINT

			auto dead_duration = std::chrono::system_clock::now() - dead_tp;

			auto years = std::chrono::duration_cast<std::chrono::years>(dead_duration);

			auto hours = std::chrono::duration_cast<std::chrono::hours>(dead_duration);
			auto min = std::chrono::duration_cast<std::chrono::minutes>(dead_duration - hours);
			auto sec = std::chrono::duration_cast<std::chrono::seconds>(dead_duration - hours - min);

			ImGui::TextColored(hl_col_negative,
				"Dead for %lld:%lld:%lld(%lld years ago)",
				(long long)hours.count(),
				(long long)min.count(),
				(long long)sec.count(),
				(long long)years.count()
			);
		}

		if (!is_alive){
			ImGui::Text("Cause of Death: ");
			ImGui::SameLine();
			ImGui::TextColored(hl_col_negative, "%s", "Suicide");
		}

		ImGui::Text("Gender: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "Male");

		ImGui::Text("Weight: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "45kg");

		ImGui::Text("Height: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "175cm");

		ImGui::Text("Spoken Languages: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "Mandarin Chinese(Native), English");

		ImGui::Text("Organizations: ");
		ImGui::SameLine();
		ImGui::TextColored(hl_col_positive, "None");

		ImGui::Text("Github Link: ");
		ImGui::SameLine();
		if(ImGui::TextLinkOpenURL("condexpr01", "https://github.com/condexpr01")){
			ctx.click();
		}
		if (hovered_once()){ctx.hover();}


	}
	ImGui::EndChild();
}

void page_profile_alpha(core::sdl_event_ctx &ctx){

	ImVec2 cra = ImGui::GetContentRegionAvail();

	float leftw_pct = 0.4f;
	float bottomh_pct = 0.6;

	ImVec2 left_size = ImVec2{cra.x * leftw_pct, cra.y * (1.f - bottomh_pct)};
	ImVec2 right_size = ImVec2{cra.x * (1.f - leftw_pct), cra.y * (1.f - bottomh_pct)};
	ImVec2 bottom_size = ImVec2{cra.x, cra.y * bottomh_pct};

	const ImVec4 hl_col_positive{0.224f, 0.773f, 0.733f, 1.000f};
	const ImVec4 hl_col_negative{1.000f, 0.000f, 0.2666667f, 1.0f};

	if(ImGui::BeginChild("profile photo", left_size)){

		int aw = 0, ah = 0;
		glGetTextureLevelParameteriv(ctx.avatar_alpha.id(), 0, GL_TEXTURE_WIDTH,  &aw);
		glGetTextureLevelParameteriv(ctx.avatar_alpha.id(), 0, GL_TEXTURE_HEIGHT, &ah);
		if (aw > 0) {
			float aspect = (float)ah / (float)aw;
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float w = avail.x;
			float h = w * aspect;
			if (h > avail.y) { h = avail.y; w = h / aspect; }
			ImGui::Image(ctx.avatar_alpha.id(), ImVec2(w, h));
		}

	}
	ImGui::EndChild();

	ImGui::SameLine();

	profile_info(ctx, right_size, hl_col_positive, hl_col_negative);

	if(ImGui::BeginChild("profile text more", bottom_size)){
		const float beat = 0.8f + 0.2f * std::sin(ImGui::GetTime() * 6.28318f);
		ImVec4 sign_col = {1.000f, 0.647f, 0.000f, 1.000f};
		sign_col.w *= beat;

		auto item = [&](const char* fmt, auto&&... args)
		{
			ImGui::TextColored(sign_col, "\u00BB");
			ImGui::SameLine(22.0f);

			if constexpr (sizeof...(args) == 0){
				ImGui::TextColored(hl_col_positive, "%s", fmt);
			}
			else{
				ImGui::TextColored(hl_col_positive, fmt, std::forward<decltype(args)>(args)...);
			}
		};

		ImGui::SeparatorText("Languages: ");
		item("c/cpp(major)");
		item("make/cmake");
		item("lua");
		item("shell");
		item("python");
		item("masm32");

		ImGui::SeparatorText("Familiar with: ");
		item("cppSTL");
		item("SDL3");
		item("Opengl(glsl)");
		item("ImGui");
		item("Pcap/Npcap");

		ImGui::SeparatorText("Preferred IMEs: ");
		item("condexpr01/rime-ranwu(五笔衍生)");
		item("english input");

		ImGui::SeparatorText("Preferred Editor: ");
		item("Neovide with condexpr01/nvim-config-lazymiku");

		ImGui::SeparatorText("Preferred OS: ");
		item("Archlinux(KDE plasma)");

		ImGui::SeparatorText("My Angel miku");
		item("Miku is really an angel.");

	}
	ImGui::EndChild();
}


void page_profile_beta(core::sdl_event_ctx &ctx){

	ImVec2 cra = ImGui::GetContentRegionAvail();

	float leftw_pct = 0.4f;
	float bottomh_pct = 0.6;

	ImVec2 left_size = ImVec2{cra.x * leftw_pct, cra.y * (1.f - bottomh_pct)};
	ImVec2 right_size = ImVec2{cra.x * (1.f - leftw_pct), cra.y * (1.f - bottomh_pct)};
	ImVec2 bottom_size = ImVec2{cra.x, cra.y * bottomh_pct};

	const ImVec4 hl_col_positive{0.224f, 0.773f, 0.733f, 1.000f};
	const ImVec4 hl_col_negative{1.000f, 0.000f, 0.2666667f, 1.0f};

	if(ImGui::BeginChild("profile photo", left_size)){

		int aw = 0, ah = 0;
		glGetTextureLevelParameteriv(ctx.avatar_beta.id(), 0, GL_TEXTURE_WIDTH,  &aw);
		glGetTextureLevelParameteriv(ctx.avatar_beta.id(), 0, GL_TEXTURE_HEIGHT, &ah);
		if (aw > 0) {
			float aspect = (float)ah / (float)aw;
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float w = avail.x;
			float h = w * aspect;
			if (h > avail.y) { h = avail.y; w = h / aspect; }
			ImGui::Image(ctx.avatar_beta.id(), ImVec2(w, h));
		}

	}
	ImGui::EndChild();

	ImGui::SameLine();

	profile_info(ctx, right_size, hl_col_positive, hl_col_negative);

	if(ImGui::BeginChild("profile text more", bottom_size)){

		const float beat = 0.8f + 0.2f * std::sin(ImGui::GetTime() * 6.28318f);
		ImVec4 sign_col = {1.000f, 0.647f, 0.000f, 1.000f};
		sign_col.w *= beat;

		auto item = [&](const char* fmt, auto&&... args)
		{
			ImGui::TextColored(sign_col, "\u00BB");
			ImGui::SameLine(22.0f);

			if constexpr (sizeof...(args) == 0){
				ImGui::TextColored(hl_col_positive, "%s", fmt);
			}
			else{
				ImGui::TextColored(hl_col_positive, fmt, std::forward<decltype(args)>(args)...);
			}
		};

		ImGui::SeparatorText("Gloomy:");
		item("厌光，厌噪，厌烦");

		ImGui::SeparatorText("Reticent:");
		item("无交流义务不想说话");

		ImGui::SeparatorText("Good person:");
		item("没有危害社会, 我是好人");

		ImGui::SeparatorText("Misc");
		item("根本没有种开发者");
		item("你只是怕了");

		ImGui::SeparatorText("I am Not a Robot!");
		if(ImGui::TextLinkOpenURL(
			"<Evidence>",
			"https://github.com/condexpr01/condexpr01/not-a-robot"
		)){
			ctx.click();
		}
		if (hovered_once()){ctx.hover();}

		int num_extra = std::size(ctx.avatar_extra);
		int aw = 0, ah = 0;
		ImVec2 avail = ImGui::GetContentRegionAvail();

		for (int i=0; i < num_extra; i++){
			glGetTextureLevelParameteriv(ctx.avatar_extra[i].id(), 0, GL_TEXTURE_WIDTH,  &aw);
			glGetTextureLevelParameteriv(ctx.avatar_extra[i].id(), 0, GL_TEXTURE_HEIGHT, &ah);

			if (aw > 0) {
				float aspect = (float)ah / (float)aw;
				float w = avail.x / num_extra;
				float h = w * aspect;
				//if (h > avail.y) { h = avail.y; w = h / aspect / num_extra; }
				ImGui::Image(ctx.avatar_extra[i].id(), ImVec2(w, h));
				if(i != num_extra - 1)ImGui::SameLine(0,0);
			}
		}


	}
	ImGui::EndChild();
}


//loop will be called at the upper level
void core::sdl_event_manager::loop() {
	prepare prepare;
	frame frame;

	prepare.all(ctx);

	while(ctx.running && ctx.status){
		frame.one_frame(ctx);
	}
}

