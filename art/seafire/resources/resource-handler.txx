namespace art::seafire::resources
{

  template<Resource R, typename F, typename... FactoryArgs >
  resource_handler_t<R, F, FactoryArgs...>::
  resource_handler_t(factory_type factory, FactoryArgs&&... args)
    : factory_{std::move(factory)},
      factory_args_{std::forward<FactoryArgs>(args)...}
  {}

  template< Resource R, typename F, typename... FactoryArgs >
  void
  resource_handler_t< R, F, FactoryArgs... >::
  on_request(server::request_t& req,
             server::response_t& res) const
  {
    auto factory_invoker = [f = factory(), &req, &res](auto... args)
    {
      return common::invoke(req, &F::template factory<resource_type>::make, f, req, res, std::forward<decltype(args)>(args)...);
    };

    auto r = std::apply(factory_invoker, factory_args_);
    handle(req, res, r);
  }

  template<Resource R,
           typename F,
           typename... FactoryArgs,
           typename H>
  H
  use_resource(F factory, FactoryArgs&&... args)
  {
    return H{std::move(factory), std::forward<FactoryArgs>(args)...};
  }

} // namespace art::seafire::resources
