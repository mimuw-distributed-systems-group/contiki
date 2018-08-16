#
# HENI: Hierarchical Embedded Network Infrastructure
#
# Copyright (c) 2016, University of Warsaw
# All rights reserved.
#

# @file
# HENI: The main make file.

.PHONY: all

$(info )
$(info HENI: Hierarchical Embedded Network Infrastructure)
$(info Embedded C libraries make system)
$(info )

HENI_SUPPORTED_PLATFORMS := $(wildcard make/*.platform.rules)
HENI_SUPPORTED_PLATFORMS := $(subst .platform.rules,,$(HENI_SUPPORTED_PLATFORMS))
HENI_SUPPORTED_PLATFORMS := $(subst make/,,$(HENI_SUPPORTED_PLATFORMS))
MAKECMDGOALS := native-gcc
ifeq ($(MAKECMDGOALS),)
$(info No platform selected. Please, select one of the following platforms:)
$(info     $(HENI_SUPPORTED_PLATFORMS))
$(info )
$(error )
else
HENI_PLATFORM_NAME := $(word 1, $(MAKECMDGOALS))
HENI_PLATFORM_GOALS := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
HENI_FOUND_PLATFORM := $(findstring $(HENI_PLATFORM_NAME),$(HENI_SUPPORTED_PLATFORMS))
ifeq ($(HENI_FOUND_PLATFORM),)
$(info Unsupported platform "$(HENI_PLATFORM_NAME)" selected. Please, select one of the following platforms:)
$(info     $(HENI_SUPPORTED_PLATFORMS))
$(info )
$(error )
endif
endif

define HENI_PLATFORM_TARGET

$(1): $(1).platform.rules
	@echo "Compiling the $(1) platform with goals: $(2)"
	$(MAKE) -f make/$$< HENI_PLATFORM_NAME=$$@ $(2)

endef

# A target that matches a specific platform.
$(eval $(call HENI_PLATFORM_TARGET,$(HENI_PLATFORM_NAME),$(HENI_PLATFORM_GOALS)))

# A target to ignore that matches everything.
%:
	@echo -n

