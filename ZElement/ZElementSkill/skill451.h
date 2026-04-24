#ifndef __CPPGEN_GNET_SKILL451
#define __CPPGEN_GNET_SKILL451
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill451:public Skill
    {
      public:
        enum
        { SKILL_ID = 451 };
          Skill451 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill451Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.1 * skill->GetT0 () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (12 - skill->GetLevel ())));
                skill->SetPlus (15 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill451Stub ():SkillStub (451)
        {
            occupation = 130;
            name = L"Éã»ê¶áÆÇ";
            nativename = "Éã»ê¶áÆÇ";
            icon = "Éã»ê¶áÆÇ.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 721;
            talent_size = 1;
            action[0] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[1] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[2] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[4] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[5] = "ºÏ»¶ÅÉ_·¨±¦_ÉãÐÄÁå";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÉåÐÄÁå";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill451Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (32 - 2 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               32 - 2 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (), 2 * skill->GetLevel (), 15 * skill->GetLevel (), 12 - skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * ((30 + 3 * skill->GetT0 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.12 + 0.09 * skill->GetT0 ());
            skill->GetVictim ()->SetXiaoxin (1);
            return true;
        }
#endif
    };
}
#endif
