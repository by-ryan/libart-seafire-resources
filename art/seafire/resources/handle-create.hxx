#ifndef art__seafire__resources__handle_create_hxx_
#define art__seafire__resources__handle_create_hxx_

#include <art/seafire/common/invoke.hxx>
#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7231/content-type.hxx>
#include <art/seafire/protocol/rfc7231/location.hxx>
#include <art/seafire/protocol/rfc7232/etag.hxx>
#include <art/seafire/protocol/rfc7232/last-modified.hxx>
#include <art/seafire/representation/representation.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/handle-get.hxx>
#include <art/seafire/resources/metadata.hxx>
#include <art/seafire/resources/preconditions.hxx>
#include <art/seafire/server/request.hxx>
#include <art/seafire/server/response.hxx>

namespace art::seafire::resources
{

  template<CreatableResource R>
  void
  handle_create(server::request_t& req,
                server::response_t& res,
                R const& resource,
                std::optional<protocol::media_type_t> const& accepted_type);

} // namespace art::seafire::resources

#include <art/seafire/resources/handle-create.txx>

#endif
