#ifndef NW_ASINGLETON_H
#define NW_ASINGLETON_H

#include <nw_pch.hpp>
#include <nw_core.hpp>

namespace NW
{
	/// ASingleton Interface for unique global classes
	template<class Type>
	class NW_API ASingleton
	{
	public:
		ASingleton(const ASingleton& rCpy) = delete;
		virtual ~ASingleton() = default;
		// --getters
		static inline Type& Get() { static Type s_instance;  return s_instance; }
		
		void operator =(ASingleton& rCpy) = delete;
		void operator =(const ASingleton& rCpy) = delete;
	protected:
		ASingleton() = default;
	};
	/// AMaster Interface for global handler classes
	/// Description:
	/// -- Manages all objects of own type
	/// -- Creates/initializes and deletes own instances
	template<class MasterT, class IType>
	class NW_API AMaster : public ASingleton<MasterT>
	{
		using Insts = std::list<RefOwner<IType>>;
		friend class ASingleton<MasterT>;
	public: // Interface Methods
		virtual ~AMaster() = default;

		// --getters
		inline const char* GetDirectory() const { return &b_strDir[0]; }
		inline IType* GetInstance(UInt32 unId) {
			Insts::iterator itInst = std::find_if(b_pInstances.begin(), b_pInstances.end(),
				[=](RefOwner<IType>& pInst)->bool {return pInst->GetId() == unId; });
			return itInst == b_pInstances.end() ? nullptr : (*itInst).get();
		}
		// --setters
		inline void SetDirectory(const char* strDir) { b_strDir = strDir; }
		inline void AddInstance(IType* pInstance) {
			if (pInstance == nullptr) return;
			if (HasInstance(pInstance->GetId())) return;
			b_pInstances.push_back(RefOwner<IType>(pInstance));
		}
		inline void RemoveInstance(UInt32 unId) {
			Insts::iterator itInst = std::find_if(b_pInstances.begin(), b_pInstances.end(),
				[=](RefOwner<IType>& pInst)->bool {return pInst->GetId() == unId; });
			if (itInst == b_pInstances.end()) return;
			b_pInstances.erase(itInst);
		}
		// -- Predicates
		inline bool HasInstance(Int32 unId) {
			Insts::iterator itInst = std::find_if(b_pInstances.begin(), b_pInstances.end(),
				[=](RefOwner<IType>& pInst)->bool {return pInst->GetId() == unId; });
			return itInst != b_pInstances.end();
		}
	protected: // Implementation Fields
		Insts b_pInstances;
		String b_strDir;
	};
}

#endif // NW_ASINGLETON_H