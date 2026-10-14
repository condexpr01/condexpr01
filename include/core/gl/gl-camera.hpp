#ifndef CORE_HEADER_GL_CAMERA_GUARD
#define CORE_HEADER_GL_CAMERA_GUARD

#include <glm/gtc/quaternion.hpp>

namespace core{

	class camera{

		public:
			glm::vec3 pos{0,0,2};
			glm::vec3 center_pos = pos + glm::vec3{0,0,-1};
			glm::vec3 up_pos     = pos + glm::vec3{0,1,0};

		public:
			glm::mat4 look_at() noexcept{
				return glm::lookAt(pos,center_pos,up_pos-pos);
			}

			glm::mat4 look_at(glm::vec3 eye,glm::vec3 center,glm::vec3 up) noexcept{
				pos = eye;
				center_pos = center;
				up_pos = eye + up;

				return look_at();
			}

			//自由视角更新姿态，但自动roll以保持right向量平行于xz平面
			void auto_roll_rotate(float dyaw, float dpitch) noexcept{

				glm::vec3 up{up_pos - pos};
				glm::vec3 front{center_pos - pos};
				glm::vec3 right{};

				//yaw
				if (dyaw){
					glm::quat qyaw = glm::angleAxis(glm::radians(-dyaw),up);

					//yaw运算后,更新front center_pos right
					front = qyaw * front;
					center_pos = front + pos;

					right = glm::cross(up, front);
				}

				//pitch
				if (dpitch){
					right = glm::cross(up, front);
					glm::quat qpitch = glm::angleAxis(glm::radians(-dpitch),right);

					//pitch运算后,更新front,up center_pos,up_pos right
					up = qpitch * up;
					front = qpitch * front;
					up_pos = up + pos;
					center_pos = front + pos;

					right = glm::cross(up, front);
				}

				//auto roll
				if (right.y != 0.f){
					//线面角 droll>=0
					float droll = glm::asin(glm::abs(glm::dot(right,glm::vec3{0,1,0}) / glm::length(right)));

					//调整方向, up.y决定上下, xz平面上往顺时针(负方向)，下往逆时针(正方向)
					if((right.y > 0.f && up.y > 0.f) || (right.y < 0.f && up.y < 0.f)){droll = -droll;}

					glm::quat qroll = glm::angleAxis(droll,front);

					//roll运算后,更新up up_pos
					up = qroll * up;
					up_pos = up + pos;
				}

			}

			//自由更新姿态
			//顺序: dyaw -> dpitch -> droll
			//使用局部的坐标系, 无万向锁
			//正：顺时针,负:逆时针
			void rotate(float dyaw, float dpitch, float droll) noexcept {

				glm::vec3 up{up_pos - pos};
				glm::vec3 front{center_pos - pos};

				//yaw
				if (dyaw){
					glm::quat qyaw = glm::angleAxis(glm::radians(-dyaw),up);

					//yaw运算后,更新front center_pos
					front = qyaw * front;
					center_pos = front + pos;
				}

				//pitch
				if (dpitch){
					glm::vec3 right = glm::cross(up, front);
					glm::quat qpitch = glm::angleAxis(glm::radians(-dpitch),right);

					//pitch运算后,更新front,up center_pos,up_pos
					up = qpitch * up;
					front = qpitch * front;
					up_pos = up + pos;
					center_pos = front + pos;
				}

				//roll
				if (droll){
					glm::quat qroll = glm::angleAxis(glm::radians(-droll),front);

					//roll运算后,更新up up_pos
					up = qroll * up;
					up_pos = up + pos;
				}

			}

			//limit in (-pi/2,pi/2)
			void limit_rotate(float dyaw, float dpitch, float limit_angle)noexcept{

				glm::vec3 up{up_pos - pos};
				glm::vec3 front{center_pos - pos};
				glm::vec3 right{};

				const float limit = 0.f;

				//yaw
				if (dyaw){
					glm::quat qyaw = glm::angleAxis(glm::radians(-dyaw),up);

					//yaw运算后,更新front center_pos right
					front = qyaw * front;
					center_pos = front + pos;

					right = glm::cross(up, front);
				}

				//pitch
				if (dpitch){
					right = glm::cross(up, front);
					glm::quat qpitch = glm::angleAxis(glm::radians(-dpitch),right);

					//pitch运算后,更新front,up center_pos,up_pos right

					glm::vec3 update_up = qpitch * up;

					//up.y wont be under xz plane
					if (update_up.y > limit){
						up = update_up;
						front = qpitch * front;
						up_pos = up + pos;
						center_pos = front + pos;

						right = glm::cross(up, front);
					}

				}

				//auto roll
				if (right.y != 0.f){
					//线面角 droll>=0
					float droll = glm::asin(glm::abs(glm::dot(right,glm::vec3{0,1,0}) / glm::length(right)));

					//调整方向, up.y决定上下, xz平面上往顺时针(负方向)，下往逆时针(正方向)
					if((right.y > 0.f && up.y > 0.f) || (right.y < 0.f && up.y < 0.f)){droll = -droll;}

					glm::quat qroll = glm::angleAxis(droll,front);

					//roll运算后,更新up up_pos
					up = qroll * up;
					up_pos = up + pos;
				}

			}


			void translate(float x, float y,float nagtive_z) noexcept{
				glm::vec3 front = glm::normalize(center_pos - pos);
				glm::vec3 up    = glm::normalize(up_pos - pos);
				glm::vec3 right = glm::normalize(glm::cross(up,-front));

				glm::mat4 translate = glm::translate(glm::mat4(1.f),x * right + y * up + nagtive_z * front);

				center_pos = translate * glm::vec4{center_pos,1.f};
				up_pos = translate * glm::vec4{up_pos,1.f};
				pos = translate * glm::vec4{pos,1.f};
			}

	};

}

#endif
