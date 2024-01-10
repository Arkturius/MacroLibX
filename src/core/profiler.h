/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiler.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:35:45 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 18:16:47 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PROFILER__
#define __MLX_PROFILER__

#include <utils/singleton.h>
#include <mlx_profile.h>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

namespace mlx
{
	using FloatingPointMilliseconds = std::chrono::duration<double, std::milli>;

	struct ProfileResult
	{
		std::string name;
		FloatingPointMilliseconds elapsed_time;
		std::thread::id thread_id;
	};

	class Profiler : public Singleton<Profiler>
	{
		friend class Singleton;

		public:
			Profiler(const Profiler&) = delete;
			Profiler(Profiler&&) = delete;

			void appendProfileData(ProfileResult&& result);

		private:
			Profiler() { beginRuntimeSession(); }
			~Profiler();

			void beginRuntimeSession();
			void writeProfile(const ProfileResult& result);
			void endRuntimeSession();
			inline void writeHeader()
			{
				_output_stream << "{\"profileData\":[{}";
				_output_stream.flush();
			}

			inline void writeFooter()
			{
				_output_stream << "]}";
				_output_stream.flush();
			}

		private:
			std::unordered_map<std::string, std::pair<std::size_t, ProfileResult>> _profile_data;
			std::ofstream _output_stream;
			std::mutex _mutex;
			bool _runtime_session_began = false;
	};

	class ProfilerTimer
	{
		public:
			ProfilerTimer(const char* name) : _name(name)
			{
				_start_timepoint = std::chrono::steady_clock::now();
			}

			inline void stop()
			{
				auto end_timepoint = std::chrono::steady_clock::now();
				auto high_res_start = FloatingPointMilliseconds{ _start_timepoint.time_since_epoch() };
				auto elapsed_time = std::chrono::time_point_cast<std::chrono::milliseconds>(end_timepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::milliseconds>(_start_timepoint).time_since_epoch();

				Profiler::get().appendProfileData({ _name, elapsed_time, std::this_thread::get_id() });

				_stopped = true;
			}

			~ProfilerTimer()
			{
				if(!_stopped)
					stop();
			}

		private:
			std::chrono::time_point<std::chrono::steady_clock> _start_timepoint;
			const char* _name;
			bool _stopped = false;
	};

	namespace ProfilerUtils 
	{
		template <std::size_t N>
		struct ChangeResult
		{
			char data[N];
		};

		template <std::size_t N, std::size_t K>
		constexpr auto cleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			std::size_t srcIndex = 0;
			std::size_t dstIndex = 0;
			while(srcIndex < N)
			{
				std::size_t matchIndex = 0;
				while(matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if(matchIndex == K - 1)
					srcIndex += matchIndex;
				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#ifdef PROFILER
	#define MLX_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::mlx::ProfilerUtils::cleanupOutputString(name, "__cdecl ");\
												::mlx::ProfilerTimer timer##line(fixedName##line.data)
	#define MLX_PROFILE_SCOPE_LINE(name, line) MLX_PROFILE_SCOPE_LINE2(name, line)
	#define MLX_PROFILE_SCOPE(name) MLX_PROFILE_SCOPE_LINE(name, __LINE__)
	#define MLX_PROFILE_FUNCTION() MLX_PROFILE_SCOPE(MLX_FUNC_SIG)
#else
	#define MLX_PROFILE_SCOPE(name)
	#define MLX_PROFILE_FUNCTION()
#endif

#endif
