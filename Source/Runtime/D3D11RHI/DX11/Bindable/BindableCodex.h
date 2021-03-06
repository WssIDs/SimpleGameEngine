#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <memory>
#include <unordered_map>
#include <type_traits>


namespace Bind
{
	class Codex
	{
	public:
		template<class T, typename...Params>
		static std::shared_ptr<T> Resolve(DX11RHI& gfx, Params&&...p)
		{
			static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
			return Get().Resolve_<T>(gfx,std::forward<Params>(p)...);
		}
	private:
		template<class T, typename...Params>
		std::shared_ptr<T> Resolve_(DX11RHI& gfx, Params&&...p)
		{
			auto key = T::GenerateUID(std::forward<Params>(p)...);
			auto i = binds.find(key);
			if (i == binds.end())
			{
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			else
			{
				return std::static_pointer_cast<T>(i->second);
			}
		}
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
	};
}

