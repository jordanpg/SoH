#ifndef FISHSANITY_H
#define FISHSANITY_H
#pragma once

#include <z64.h>
#include "randomizerTypes.h"

typedef struct FishsanityPondOptions {
    u8 mode;
    u8 numFish;
    bool ageSplit;
} FishsanityPondOptions;

typedef enum FishsanityOptionsSource {
    FSO_SOURCE_RANDO,
    FSO_SOURCE_CVARS
};

typedef enum FishsanityCheckType {
    FSC_NONE,
    FSC_POND,
    FSC_GROTTO
};

/**
 * @brief Fishsanity-related metadata for fishing pond fish
 * TODO: THERE'S NO POINT!!!!!!!! I THINK WE CAN JUST USE FISHIDENTITY POGS POGS POGS
*/
typedef struct FishsanityMeta {
    s16 params;
    bool killAfterCollect;
    FishIdentity fish;
} FishsanityMeta;

#ifdef __cplusplus
namespace Rando {

/**
 * @brief Class to provide an interface for and direct Fishsanity features
*/
class Fishsanity {
  public:
    Fishsanity();
    ~Fishsanity();

    static const FishIdentity defaultIdentity;
    static const FishsanityMeta defaultMeta;

    /**
     * @brief Gets the type of a fishsanity check
     * @param rc The RandomizerCheck to categorize
     * @return The check's fishsanity type, or FSC_NONE
     */
    static FishsanityCheckType GetCheckType(RandomizerCheck rc);

    /**
     * @brief Returns true if the given fish location is active
     * 
     * @param loc The Location to check
     * @param optionsSource Optionally declare from which source to pull settings
    */
    bool GetFishLocationIncluded(Rando::Location* loc, FishsanityOptionsSource optionsSource = FSO_SOURCE_RANDO);

    /**
     * @brief Get the active and inactive locations in the fishing pond.
     * 
     * @param optionsSource Optionally declare from which source to pull settings
     * @return A pair of vectors, where the fist is all active pond fish checks, and the second is all inactive pond fish checks.
    */
    std::pair<std::vector<RandomizerCheck>, std::vector<RandomizerCheck>> GetFishingPondLocations(FishsanityOptionsSource optionsSource = FSO_SOURCE_RANDO);

    /**
     * @brief Get all active fishsanity locations, and all inactive fishing pond locations.
     * 
     * @param optionsSource Optionally declare from which source to pull settings 
     * @return A pair of vectors, where the first is all active fishsanity checks, and the second is all inactive fishsanity checks.
    */
    std::pair<std::vector<RandomizerCheck>, std::vector<RandomizerCheck>> GetFishsanityLocations(FishsanityOptionsSource optionsSource = FSO_SOURCE_RANDO);

    /**
     * @brief Returns the identity for a caught pond fish given its params.
     * Not for use externally from rando, use Randomizer::IdentifyFish or Randomizer_IdentifyFish for that
     * 
     * @param fishParams Actor parameters for the fish to identify
    */
    FishIdentity IdentifyPondFish(u8 fishParams);

    /**
     * @brief Get fishsanity fishing pond options from the requested source
    */
    FishsanityPondOptions GetOptions(FishsanityOptionsSource optionsSource = FSO_SOURCE_RANDO);

    /**
     * @brief Get metadata for a fish given its params
     * 
     * @param params Actor parameters for the pond fish
     * @param current Current metadata associated with this fish, if any. 
    */
    FishsanityMeta GetPondFishMetaFromParams(s16 params);
    
    /**
     * @brief Updates current pond fish according to save data
    */
    void UpdateCurrentPondFish();
    
    /**
     * @brief Initializes internal state from save
    */
    void InitializeFromSave();

    /**
     * @brief Returns true if the fishing pond is shuffled
    */
    bool GetPondFishShuffled();

    /**
     * @brief Returns true if grotto fish are shuffled 
    */
    bool GetGrottoFishShuffled();

    /**
     * @brief Returns true if the fishing pond is currently adult (i.e., age split is enabled and Link is adult)
    */
    bool IsAdultPond();

    /**
     * @brief Returns true if all available pond fish checks have been collected for the current age
    */
    bool GetPondCleared();

    /**
     * @brief Advances current fishing pond check; no effect if every fish is shuffled
     * @return The new FishsanityMeta for the current pond, or default metadata if every fish is shuffled
    */
    FishsanityMeta AdvancePond();

    /**
     * @brief Set the currently held fish
     * @param meta Pointer to FishsanityMeta to copy
    */
    void SetHeldFish(FishsanityMeta* meta);

    /**
     * @brief Get the currently held fish
    */
    FishsanityMeta GetHeldFish();

  private:
    /**
     * @brief Initialize helper statics if they have not been initialized yet
    */
    void InitializeHelpers();
    
    /**
     * @brief Resolves a pond fish's FishIdentity directly from params & pond age
     * 
     * @param params Params for Fishing actor
     * @param adultPond Whether to resolve this fish as an adult check
     * @return The FishIdentity for the described fish
    */
    static FishIdentity GetPondFish(s16 params, bool adultPond);
    
    /**
     * @brief Current pond fish when all pond fish are not randomized
    */
    std::pair<FishsanityMeta, FishsanityMeta> mCurrPondFish;
    
    /**
     * @brief Metadata for the currently held fish in the fishing pond minigame
    */
    FishsanityMeta mHeldMetadata;
    
    /**
     * @brief True if fishsanity helpers have been initialized
    */
    static bool fishsanityHelpersInit;

    /////////////////////////////////////////////////////////
    //// Helper data structures derived from static data ////
    /////////////////////////////////////////////////////////

    /**
     * @brief Mapping from pond fish check to the age where that check can be collected
    */
    static std::unordered_map<RandomizerCheck, LinkAge> pondFishAgeMap;

    /**
     * @brief List of child pond fish checks
    */
    static std::vector<RandomizerCheck> childPondFish;

    /**
     * @brief List of adult pond fish checks
    */
    static std::vector<RandomizerCheck> adultPondFish;
};
}

extern "C" {
#endif
/// Returns true if pond fish should be shuffled based on fishsanity settings.
bool Randomizer_GetPondFishShuffled();
/// Returns true if grotto fish should be shuffled based on fishsanity settings.
bool Randomizer_GetGrottoFishShuffled();
/// Returns true if the adult fishing pond should be used for fishsanity.
bool Randomizer_IsAdultPond();
/// Sets the currently-held fish
void Randomizer_SetHeldFish(FishsanityMeta* meta);
#ifdef __cplusplus
}
#endif

#endif // FISHSANITY_H