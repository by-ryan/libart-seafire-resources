namespace art::seafire::resources
{

  template<ErasableResource R>
  void
  handle_erase(server::request_t& req,
               server::response_t& res,
               R const& resource)
  {
    common::invoke(resource, req, &R::erase);
    res.send(204);
  }

} // namespace art::seafire::resources
