#ifndef art__seafire__resources__resource_handler_hxx_
#define art__seafire__resources__resource_handler_hxx_

#include <art/seafire/common/invoke.hxx>
#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7231/accept.hxx>
#include <art/seafire/protocol/rfc7231/content-type.hxx>
#include <art/seafire/protocol/rfc7232/entity-tag.hxx>
#include <art/seafire/protocol/rfc7232/etag.hxx>
#include <art/seafire/protocol/rfc7232/last-modified.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/handle.hxx>
#include <art/seafire/resources/traits.hxx>
#include <art/seafire/server/request.hxx>
#include <art/seafire/server/response.hxx>

#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace art::seafire::resources
{

  template<Resource R,
           typename F, // TODO: Implement factory concept
           typename... FactoryArgs>
  class resource_handler_t {
  public:
    using resource_type = R;
    using factory_type = F;

    explicit
    resource_handler_t(factory_type factory, FactoryArgs&&... args);

    factory_type const&
    factory() const
    {
      return factory_;
    }

    void
    on_request(server::request_t& req, server::response_t& res) const;

    void
    operator()(server::request_t& req, server::response_t& res) const
    {
      on_request(req, res);
    }

  private:
    factory_type factory_;
    std::tuple<FactoryArgs...> factory_args_;
  };

  template<Resource R,
           typename F,
           typename... FactoryArgs,
           typename H = resource_handler_t<R, F, FactoryArgs...>>
  H use_resource(F factory, FactoryArgs&&... args);

} // namespace art::seafire::resources

#include <art/seafire/resources/resource-handler.txx>

#endif
