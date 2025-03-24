#ifndef art__seafire__resources__handle_erase_hxx_
#define art__seafire__resources__handle_erase_hxx_

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

  template<ErasableResource R>
  void
  handle_erase(server::request_t& req,
               server::response_t& res,
               R const& resource);

} // namespace art::seafire::resources

#include <art/seafire/resources/handle-erase.txx>

#endif
