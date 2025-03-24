#ifndef art__seafire__resources__metadata_hxx_
#define art__seafire__resources__metadata_hxx_

#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7232/entity-tag.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/traits.hxx>

#include <art/seafire/representation/concepts.hxx>
#include <art/seafire/representation/representation.hxx>
#include <art/seafire/representation/traits.hxx>

#include <art/uri/uri.hxx>

#include <chrono>
#include <optional>

namespace art::seafire::resources
{

  // location
  //
  template<Resource R>
  std::optional<uri::uri_t>
  get_location(R const& res)
  {
    using local_traits = traits::resource_traits<R>;

    if constexpr (local_traits::has_location)
      return res.location();

    return std::nullopt;
  }


  // etag
  //
  template<representation::BasicRepresentation BR>
  std::optional<protocol::rfc7232::entity_tag_t>
  get_etag(BR const& rep)
  {
    using local_traits = representation::traits::representation_traits<BR>;

    if constexpr (local_traits::has_entity_tag_t)
      return rep.etag();

    return std::nullopt;
  }

  inline
  std::optional<protocol::rfc7232::entity_tag_t>
  get_etag(representation::representation_t const& rep)
  {
    return rep.etag();
  }

  template<Resource R>
  std::optional<protocol::rfc7232::entity_tag_t>
  get_etag(R const& res)
  {
    using local_traits = traits::resource_traits<R>;

    if constexpr (local_traits::has_entity_tag)
      return res.etag();

    return std::nullopt;
  }

  // last-modified
  //
  template<representation::BasicRepresentation BR>
  std::optional<std::chrono::system_clock::time_point>
  get_last_modified(BR const& rep)
  {
    using local_traits = representation::traits::representation_traits<BR>;

    if constexpr (local_traits::has_last_modified)
      return rep.last_modified();

    return std::nullopt;
  }

  inline
  std::optional<std::chrono::system_clock::time_point>
  get_last_modified(representation::representation_t const& rep)
  {
    return rep.last_modified();
  }

  template<Resource R>
  std::optional<std::chrono::system_clock::time_point>
  get_last_modified(R const& res)
  {
    using local_traits = traits::resource_traits<R>;

    if constexpr (local_traits::has_last_modified)
      return res.last_modified();

    return std::nullopt;
  }

} // namespace art::seafire::resources

#endif
