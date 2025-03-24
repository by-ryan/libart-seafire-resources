#ifndef art__seafire__resources__preconditions_hxx_
#define art__seafire__resources__preconditions_hxx_

#include <art/seafire/protocol/rfc7232/if-match.hxx>
#include <art/seafire/protocol/rfc7232/if-modified-since.hxx>
#include <art/seafire/protocol/rfc7232/if-none-match.hxx>
#include <art/seafire/protocol/rfc7232/if-unmodified-since.hxx>
#include <art/seafire/resources/concepts.hxx>
#include <art/seafire/resources/traits.hxx>
#include <art/seafire/server/request.hxx>
#include <art/seafire/server/response.hxx>

#include <art/seafire/representation/concepts.hxx>
#include <art/seafire/representation/traits.hxx>

#include <chrono>

namespace art::seafire::resources
{

  bool
  check_preconditions(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r);

  // If-Match
  //
  bool
  check_if_match(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    protocol::rfc7232::if_match_t const& if_match);

  // If-Unmodified-Since
  //
  bool
  check_if_unmodified_since(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& representation,
    std::chrono::system_clock::time_point const& if_unmodified_since);

  // If-None-Match
  //
  bool
  check_if_none_match(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    protocol::rfc7232::if_none_match_t const& if_none_match);

  // If-Modified-Since
  //
  bool
  check_if_modified_since(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    std::chrono::system_clock::time_point const& if_modified_since);

} // namespace art::seafire::resources

#include <art/seafire/resources/preconditions.txx>

#endif
