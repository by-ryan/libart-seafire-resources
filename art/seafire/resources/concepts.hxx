#ifndef art__seafire__resources__concepts_hxx_
#define art__seafire__resources__concepts_hxx_

#include <art/seafire/resources/traits.hxx>

namespace art::seafire::resources
{

  // resources.
  //

  template<typename R>
  concept GettableResource = traits::is_gettable_resource_v<R>;

  template<typename R>
  concept UpdatableResource = traits::is_updatable_resource_v<R>;

  template<typename R>
  concept CreatableResource = traits::is_creatable_resource_v<R>;

  template<typename R>
  concept ErasableResource = traits::is_erasable_resource_v<R>;

  template<typename R>
  concept Resource
    = traits::is_gettable_resource_v<R>
    || traits::is_updatable_resource_v<R>
    || traits::is_creatable_resource_v<R>
    || traits::is_erasable_resource_v<R>;

} // namespace art::seafire::resources

#endif
