# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
 - either function
 - maybe function
 - value_or method for Maybe

## [0.1.1] - 2020-11-08

### Changed
- Use only regular `mappend` and `mempty` instead of static functions
  It is a API change, but

```
Major version zero (0.y.z) is for initial development. Anything MAY change at any time. The public API SHOULD NOT be considered stable.
```
It is rather small refactoring change, instead of intentional backward compatibility breaking change

## [0.1.0] - 2020-11-08
### Added
- Maybe
- Either
- fmap for Maybe
- fmap for Either
- Monoid: 
  - All, 
  - Any, 
  - MinInt, 
  - MaxInt

### Changed

### Deprecated

### Removed

### Fixed

### Security