#ifndef art__seafire__resources__handle_hxx_
#define art__seafire__resources__handle_hxx_

#include <art/seafire/protocol/rfc7231/allow.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/handle-create.hxx>
#include <art/seafire/resources/handle-erase.hxx>
#include <art/seafire/resources/handle-get.hxx>
#include <art/seafire/resources/handle-update.hxx>
#include <art/seafire/resources/negotiate.hxx>
#include <art/seafire/resources/traits.hxx>
#include <art/seafire/server/request.hxx>
#include <art/seafire/server/response.hxx>

#include <sstream>
#include <type_traits>

namespace art::seafire::resources
{

  void
  handle(server::request_t& req,
         server::response_t& res,
         Resource auto const& r);

} // namespace art::seafire::resources

#include <art/seafire/resources/handle.txx>

#endif
