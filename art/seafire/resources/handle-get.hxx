#ifndef art__seafire__resources__handle_get_hxx_
#define art__seafire__resources__handle_get_hxx_

#include <art/seafire/common/invoke.hxx>
#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7231/content-type.hxx>
#include <art/seafire/protocol/rfc7231/location.hxx>
#include <art/seafire/protocol/rfc7232/etag.hxx>
#include <art/seafire/protocol/rfc7232/last-modified.hxx>
#include <art/seafire/representation/representation.hxx>
#include <art/seafire/representation/select.hxx>
#include <art/seafire/representation/send.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/metadata.hxx>
#include <art/seafire/resources/preconditions.hxx>
#include <art/seafire/server/transaction.hxx>

#include <optional>

namespace art::seafire::resources
{

  enum class get_kind_t {
    get,
    head,
    created_resource,
    updated_resource
  };

  template<GettableResource R>
  void
  handle_get(server::request_t& req,
             server::response_t& res,
             R const& resource,
             std::optional<protocol::media_type_t> const& accepted_type,
             get_kind_t kind);

} // namespace art::seafire::resources

#include <art/seafire/resources/handle-get.txx>

#endif
