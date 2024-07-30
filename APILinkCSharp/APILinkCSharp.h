#pragma once

namespace APILinkCSharp
{
public

	ref class BaiduTranslateAPI
	{
	public:
		BaiduTranslateAPI();
		~BaiduTranslateAPI();

		System::String ^ Translate(System::String ^ source, System::String ^ from, System::String ^ to);
		System::String ^ GetLanguages();
	};
}  // namespace APILinkCSharp
