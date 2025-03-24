#ifndef art__seafire__resources__negotiate_hxx_
#define art__seafire__resources__negotiate_hxx_

#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7231/accept.hxx>
#include <art/seafire/representation/concepts.hxx>
#include <art/seafire/representation/traits.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/traits.hxx>
#include <art/seafire/server/request.hxx>
#include <art/seafire/server/response.hxx>

#include <optional>

namespace art::seafire::resources
{

// fixme: implement concept for handler.

  template<representation::Representation R, typename Handler>
  void
  negotiate(server::request_t&, server::response_t&, Handler&&);

} // namespace art::seafire::resources

#include <art/seafire/resources/negotiate.txx>

#endif
