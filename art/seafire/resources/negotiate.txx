namespace art::seafire::resources
{

  template<representation::Representation R, typename Handler>
  void
  negotiate(server::request_t& req, server::response_t& res, Handler&& handler)
  {
    using representation_traits = representation::traits::representation_traits<R>;

    if constexpr (representation_traits::is_content_negotiable) {
      using protocol::rfc7231::accept_t;

      if (auto opt_accept = get<accept_t>(req); opt_accept) {
        auto const accept = *opt_accept;

        for (auto const& accepted_type : accept) {
          if (R::is_accepted(accepted_type)) {
            handler(accepted_type);
            return;
          }
        }

        res.send(server::common_error_t::not_acceptable);
        return;
      }
    }

    handler(std::nullopt);
  }

} // namespace art::seafire::resources
