#pragma once
#include <string>

namespace Fiea::GameEngine
{
	template<class DerivedType>
	class IFactory
	{
	public:
		/// @brief Return the address of a concrete product associated with this concrete factory class
		/// @return DerivedType* (usually just Scope*)
		virtual DerivedType* Create() const = 0;

		/// @brief Return a string representing the name of the class the factory instantiates.
		/// @return class name string
		virtual std::string ClassName() const = 0;
	};

#define MAKE_FACTORY(_Base, _Concrete) \
    class _Concrete##Factory : public IFactory<_Base> { \
    public: \
        _Base* Create() const override { \
            return FNEW _Concrete(); \
        } \
        \
        std::string ClassName() const override { \
            return #_Concrete; \
        } \
    };
}