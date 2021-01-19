#ifndef NW_ENTITY_SYSTEM_H
#define NW_ENTITY_SYSTEM_H

namespace NW
{
	/// Abstract Entity class
	class NW_API AEntity
	{
	public:
		AEntity() = default;
		AEntity(const AEntity& rCpy) = delete;
		virtual ~AEntity() = default;

		// --getters
		inline UInt32 GetId() const { return m_unId; }
		virtual inline UInt32 GetStaticId() const = 0;
	protected:
		UInt32 m_unId;
	};
	template <class EType>
	class NW_API Entity : public AEntity
	{
	public:
		Entity() = default;
		Entity(const Entity<EType>& rCpy) = delete;
		virtual ~Entity() = default;
		virtual inline UInt32 GetStaticId() override {}
	private:
		static const UInt32 s_TypeId;
	};
	template <class EType> inline const UInt32 Entity<EType>::s_TypeId = util::internal::FamilyTypeId::Get();
	/// EntitySystem static class
	class NW_API EntSys
	{
		template <class CmpType> using Cmps = HashMap<UInt32, CmpType>;
	public:
		// --getters
		// --setters
		// --core_methods
		bool OnInit();
		void OnQuit();

	private:
		static IdStack s_IdStack;
	};
}

#endif	// NW_ENTITY_SYSTEM_H