#pragma once
#include <unordered_map>
#include "Signature.h"

namespace Fiea::GameEngine
{
	class TypeRegistry
	{
    public:

        // Static method to get the singleton instance
        static TypeRegistry& CreateInstance() {
            if (instance == nullptr)
            {
                instance = new TypeRegistry();
            }
            else
            {
                throw std::logic_error("TypeReg instance already exists");
            }
            return *instance;
        }

        /// @brief Delete singleton instance
        static void DeleteInstance()
        {
            delete instance;
            instance = nullptr;
        }

        /// @brief populates that class with it's prescribed attributes
        /// @param id 
        /// @param sigs 
        void PopulateMap(size_t id, vector<Signature> sigs)
        {
            _sigs[id] = sigs;
        }

        /// @brief Get prescribed attributes for id and return them as a vector
        /// @param id 
        /// @return 
        vector<Signature> GetSignatures(size_t id)
        {
            auto it = _sigs.find(id);
            if (it != _sigs.end())
            {
                return (it->second);
            }
            else
            {
                throw std::logic_error("Signatures for this ID don't exist!");
            }
        }

        inline static TypeRegistry* instance;
    private:
        TypeRegistry() { }

        // delete copy const and operator= because we may only have one instance at a time
        TypeRegistry(const TypeRegistry&) = delete;
        TypeRegistry& operator=(const TypeRegistry&) = delete;

        // map that holds all attributes of child class
        std::unordered_map<size_t, vector<Signature>> _sigs{};
	};
}